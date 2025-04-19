/**
 * @brief A place for globals
 */
#ifndef APP_GLOBALS_H__
#define APP_GLOBALS_H__
#include <gtk/gtk.h>
#include "views/composites/alarms_content/alarms_content.h"
#include "views/composites/setup_wizard/sequence_runner.h"

G_BEGIN_DECLS

#define DEFAULT_TIMESTAMP_LOG_FORMAT "%04d-%02d-%02dT%02d:%02d:%02d"

typedef enum {
   RVALUE_SUCCESS,
   RVALUE_ERROR
} RVALUE;

typedef enum {
   LOGLEVEL_DEBUG,
   LOGLEVEL_INFO,
   LOGLEVEL_ERROR
} LOGLEVEL;

typedef enum
{
   // Should this be a hi->lo, or lo->hi, can't tell right now (03/23/25)
   NO_ALARMS = 0,
   ALARM_LOW,
   ALARM_MID,
   ALARM_HIGH,
   NUM_ALARM_LEVELS
} ALARM_LEVELS;

extern const char* DEBUG_STR;
extern const char* INFO_STR;
extern const char* ERROR_STR;
extern const int   LOGGING_MAX_MSG_LENGTH;

typedef struct {
   GtkWidget   *w_msg_out_textview;
   GtkWidget   *w_say_something_entry;

   GtkWidget   *w_app_content_box;
   GtkWidget   *w_box_sandbox_content;

   AlarmsContent  *w_alarms_content_root;
   SequenceRunner *w_sequence_runner;

} app_widget_ref_struct;

extern app_widget_ref_struct *g_app_widget_refs;

G_END_DECLS
#endif  /* APP_GLOBALS_H__ */
