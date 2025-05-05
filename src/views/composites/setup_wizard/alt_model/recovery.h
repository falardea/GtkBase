#ifndef INC_RECOVERY_H_
#define INC_RECOVERY_H_
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

bool readStringFromFileIfExists(char *value, size_t length, const char *filename);
void saveBoolToFile(bool value, const char *filename);
bool readBoolFromFileIfExists(volatile bool *value, const char *filename);
void saveStringToFile(const char *value, const char *filename);
void saveLongToFile(long value, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* INC_RECOVERY_H_ */
