/**
 * @brief A place for globals
 */
#ifndef APP_GLOBALS_H__
#define APP_GLOBALS_H__
#include <gtk/gtk.h>
#include "gtk_composites/log_terminal.h"
#include "gtk_composites/app_mode_selector.h"
#include "gtype_composites/app_model.h"

#define DEFAULT_TIMESTAMP_LOG_FORMAT "%04d-%02d-%02dT%02d:%02d:%02d"

#define HW_SIM_PERSISTS 0
#if HW_SIM_PERSISTS
#include "gtk_composites/hw_test_panel/hw_sim_panel.h"
#endif

typedef enum {
   RVALUE_SUCCESS,
   RVALUE_ERROR
} RVALUE;

typedef enum {
   LOGLEVEL_TRACE,
   LOGLEVEL_DEBUG,
   LOGLEVEL_INFO,
   LOGLEVEL_ERROR
} LOGLEVEL;

extern const char* DEBUG_STR;
extern const char* INFO_STR;
extern const char* ERROR_STR;
extern const int   LOGGING_MAX_MSG_LENGTH;

typedef struct {
   GtkApplicationWindow *main_wnd;
   GtkOverlay           *app_wnd_overlay;

   GtkWidget   *w_msg_out_textview;
   GtkWidget   *w_say_something_entry;

   GtkWidget   *w_sandbox_content;
   AppModeSelector *mode_selector;


   GtkWidget   *app_gutter;
   LogTerminal *msg_out;

   AppModel    *app_model;

#if HW_SIM_PERSISTS
   HwSimPanel *hw_sim_panel;
#endif

} app_widgets;

extern app_widgets *g_app_widgets;
app_widgets *get_app_widgets_pointer(void);

#endif  /* APP_GLOBALS_H__ */
