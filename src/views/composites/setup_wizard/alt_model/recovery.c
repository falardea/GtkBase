#define _GNU_SOURCE

#include <libgen.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "halo_shared_variables.h"
#include "configuration_state.h"
#include "recovery.h"
#include "utils.h"
#include "logs.h"

#define READ_BUF_SIZE 128

typedef bool (*converter)(char *line, volatile void *value);
typedef int (*printer)(const void *, char **);

//static GAsyncQueue *queue = NULL;
//static GHashTable  *hashTable = NULL;

static void replaceNewline(char *str, ssize_t size)
{
   if (str == NULL)
   {
      return;
   }

   if (str[size - 1] == '\n')
   {
      str[size - 1] = '\0';
   }
}
static bool getlineString(char *buf, size_t length, FILE *fp, const char *filename)
{
   int    nextChar;
   size_t pos = 0;
   while (true)
   {
      nextChar = fgetc(fp);
      if (((nextChar == EOF) && feof(fp)) || ((char) nextChar == '\n'))
      {
         // End of file means end of the string we are reading
         buf[pos] = '\0';
         return true;
      }
      else if (ferror(fp))
      {
         // Error reading from the file
         g_print("Error reading from %s\n", filename);
         buf[0] = '\0';
         return false;
      }
      else if (pos == length - 1)
      {
         // This was the last character in the buffer, but not the end of the file or a newline
         g_print("Out of space to read string from %s (at last position %zu in buffer and read %c)\n",
                 filename, pos, (char) nextChar);
         buf[0] = '\0';
         return false;
      }
      else
      {
         // A valid character, write it into the buffer
         buf[pos] = (char) nextChar;
         ++pos;
      }
   }
}
static void checkStringRaiseAlarmOnError(const char *s1, const char *s2, const char *filename, bool s2WasRead)
{
   if (!s2WasRead || (s2 == NULL) || (strcmp(s1, s2) != 0))
   {
      g_print("Corrupt data in file: '%s'", filename);
   }
}
static bool convert_to_bool(char *line, volatile void *value)
{
   if (strcasecmp(line, "true") == 0)
   {
      *((volatile bool *) value) = true;
      return true;
   }
   else if (strcasecmp(line, "false") == 0)
   {
      *((volatile bool *) value) = false;
      return true;
   }
   else
   {
      g_print("Unexpected file contents '%s' in convert_to_bool\n", line);
      return false;
   }
}
static int print_string(const void *value, char **str) { return asprintf(str, "%s",         (const char *)     value); }
static int print_long_ptr(const void *value, char **str) { return asprintf(str, "%ld",      *((const long *)     value)); }
static int print_bool_ptr(const void *value, char **str) { return asprintf(str, "%s",       *((const bool *)     value) ? "true" : "false"); }
static bool readValFromFileIfExists(volatile void *value, converter convert_func, const char *filename)
{
   // If the file does not exist, this is not an error, but we have nothing to do
   if (!fileExists(filename))
   {
      //Not an error
      return false;
   }

   // Open the file
   FILE *fp = fopen(filename, "r");
   if (fp == NULL)
   {
      g_perror("fopen %s", filename);
      return false;
   }

   char    *line1 = NULL;
   char    *line2 = NULL;
   size_t  len = 0;
   ssize_t rc1 = getline(&line1, &len, fp);
   if (rc1 < 0)
   {
      // Failed to read a line, this is an error, leave the value alone
      g_perror("getline %s", filename);
      free(line1);
      fclose(fp);
      return false;
   }
   ssize_t rc2 = getline(&line2, &len, fp);
   // Done with the file now
   fclose(fp);

   // If the line ended with a newline, replace it with a null terminator
   replaceNewline(line1, rc1);

   bool line2Good = rc2 > -1;
   if (!line2Good)
   {
      free(line2);
      line2 = NULL;
   }
   else
   {
      replaceNewline(line2, rc2);
   }

   checkStringRaiseAlarmOnError(line1, line2, filename, line2Good);

   // Convert line 1 to the relevant type even if line 2 was bad
   bool status = convert_func(line1, value);

   // Free the lines and return whether the conversion succeeded
   free(line1);

   if (line2 != NULL)
   {
      free(line2);
   }

   if (!status)
   {
      g_print("%s: conversion failed", __func__);
   }

   return status;
}

static bool atomicSaveToFile(const char *value, const char *filename)
{
   // Get a temporary filename to write to
   char tmpFilename[128];
   int  rc = snprintf(tmpFilename, sizeof(tmpFilename), "%s.tmp", filename);
   if (rc < 0)
   {
      fprintf(stderr, "snprintf %s.tmp: %s\n", filename, strerror(errno));
      return false;
   }
   else if ((size_t) rc >= sizeof(tmpFilename))
   {
      fprintf(stderr, "Filename %s.tmp was too long\n", filename);
      return false;
   }

   // Open the file
   FILE *fp = fopen(tmpFilename, "w");
   if (fp == NULL)
   {
      fprintf(stderr, "fopen %s: %s\n", tmpFilename, strerror(errno));
      return false;
   }

   // If the provided value was not NULL, then write it to the file (a write
   // with a NULL value will just create an empty file with a crurent timestamp)
   if (value != NULL)
   {
      // Write the value twice on two separate lines
      rc = fprintf(fp, "%s\n%s\n", value, value);
      if (rc < 0)
      {
         fprintf(stderr, "fprintf %s: %s\n", tmpFilename, strerror(errno));
         fclose(fp);
         return false;
      }

      // Flush the write from the file pointer to the OS (not necessarily to disk yet)
      rc = fflush(fp);
      if (rc != 0)
      {
         fprintf(stderr, "fflush %s: %s\n", tmpFilename, strerror(errno));
         fclose(fp);
         return false;
      }
   }

   // Check for errors on the stream (probably redundant since we check the
   // return values from the fprintf and fflush calls)
   if (ferror(fp) != 0)
   {
      fprintf(stderr, "Error indicator set when trying to write %s\n", tmpFilename);
      fclose(fp);
      return false;
   }

   bool isFileSaved = true;

   // Get the file descriptor for the file pointer
   int fd = fileno(fp);
   if (fd == -1)
   {
      fprintf(stderr, "fileno %s: %s\n", tmpFilename, strerror(errno));
      isFileSaved = false;
   }

   // Close the file
   rc = fclose(fp);
   if (rc != 0)
   {
      fprintf(stderr, "fclose %s: %s\n", tmpFilename, strerror(errno));
      // Close may be responsible for actually flushing buffers to disk, in
      // which case it can fail for the same reasons write can fail (e.g. a
      // full file system), which might leave the file in an invalid state.
      // In practice with the previous fflush call, this shouldn't happen,
      // but better safe than sorry
      return false;
   }

   // Atomically rename the temporary file to have the real file name
   rc = rename(tmpFilename, filename);
   if (rc != 0)
   {
      fprintf(stderr, "rename %s -> %s: %s\n", tmpFilename, filename, strerror(errno));
      return false;
   }

   return isFileSaved;
}
static void formatAndEnqueue(const void *valPtr, printer print_func, const char *filename)
{
   // Format the provided value into a human-readable string
   char *value = NULL;
   int  rc = print_func(valPtr, &value);
   if (rc < 0)
   {
      fprintf(stderr, "asprintf for %s: %s\n", filename, strerror(errno));
      return;
   }

   char *key = strdup(filename);
   if (key == NULL)
   {
      fprintf(stderr, "strdup %s: %s\n", filename, strerror(errno));
      free(value);
      return;
   }
   atomicSaveToFile(value, filename);
}


bool readStringFromFileIfExists(char *value, size_t length, const char *filename)
{
   // If the file does not exist, this is not an error, but we have nothing to do
   if (!fileExists(filename))
   {
      return false;
   }

   // If there is no space, we can't read a string
   if (length == 0)
   {
      return false;
   }

   // Open the file
   FILE *fp = fopen(filename, "r");
   if (fp == NULL)
   {
      g_perror("fopen %s", filename);
      return false;
   }

   char line1[READ_BUF_SIZE] = { 0 };
   char line2[READ_BUF_SIZE] = { 0 };

   // Read first line
   if (!getlineString(line1, sizeof(line1), fp, filename))
   {
      // If we can't read the first line then we can't do anything else
      fclose(fp);
      g_print("%s: failed to read line", __func__);
      return false;
   }
   bool res = getlineString(line2, sizeof(line2), fp, filename);
   checkStringRaiseAlarmOnError(line1, line2, filename, res);
   fclose(fp);

   // Copy the first line of the file to value
   int bytesWritten = snprintf(value, length, "%s", line1);
   if (bytesWritten < 0)
   {
      int errnoCopy = errno;

      g_print("%s: %d", __func__, errnoCopy);
      value[0] = '\0';
      return false;
   }
   if (((size_t) bytesWritten) >= length)
   {
      g_print("%s: bytes written %d", __func__, bytesWritten);
      return false;
   }
   return true;
}
bool readBoolFromFileIfExists(volatile bool *value,     const char *filename) { return readValFromFileIfExists(value, convert_to_bool,  filename); }

void saveBoolToFile(bool value,        const char *filename) { formatAndEnqueue(&value, print_bool_ptr,  filename); }
void saveStringToFile(const char *value, const char *filename) { formatAndEnqueue(value, print_string,    filename); }
void saveLongToFile(long value,        const char *filename) { formatAndEnqueue(&value, print_long_ptr,  filename); }
