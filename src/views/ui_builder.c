/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "ui_builder.h"
#include "utils/logging.h"
#include "gtk_composites/log_terminal.h"
#include "views/color_picker/color_picker.h"

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;

   app_widget_ref_struct *appWidgetsT = g_slice_new0(app_widget_ref_struct);

   builder = gtk_builder_new();

   if (gtk_builder_add_from_resource(builder, "/resource_path/main_app_wnd", NULL) == 0) {
      logging_llprintf(LOGLEVEL_ERROR, "failed to load glade resource");
      return NULL;
   }

   appWidgetsT->w_msg_out_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_out_textview"));
   appWidgetsT->w_say_something_entry = GTK_WIDGET(gtk_builder_get_object(builder, "say_something_entry"));

   appWidgetsT->app_gutter = GTK_WIDGET(gtk_builder_get_object(builder, "app_gutter"));

   appWidgetsT->msg_out = log_terminal_new();

   gtk_box_pack_start(GTK_BOX(appWidgetsT->app_gutter), GTK_WIDGET(appWidgetsT->msg_out), TRUE, TRUE, 0);

   appWidgetsT->w_sandbox_content = GTK_WIDGET(gtk_builder_get_object(builder, "sandbox_content"));
   appWidgetsT->w_picker = color_picker_new();
   gtk_box_pack_start(GTK_BOX(appWidgetsT->w_sandbox_content), GTK_WIDGET(appWidgetsT->w_picker), TRUE, TRUE, 0);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}
