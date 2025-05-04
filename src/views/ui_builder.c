/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "ui_builder.h"
#include "utils/logging.h"
// #include "gtk_composites/log_terminal.h"

app_widget_ref_struct *get_app_widgets_pointer(void)
{
   return g_app_widget_refs;
}

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;

   app_widget_ref_struct *appWidgetsT = g_slice_new(app_widget_ref_struct);

   builder = gtk_builder_new();

   if (gtk_builder_add_from_resource(builder, "/resource_path/main_app_wnd", NULL) == 0) {
      logging_llprintf(LOGLEVEL_ERROR, "failed to load glade resource");
      return NULL;
   }

   appWidgetsT->main_wnd = GTK_APPLICATION_WINDOW(gtk_builder_get_object(builder, "main_wnd"));
   appWidgetsT->app_wnd_overlay = GTK_OVERLAY(gtk_builder_get_object(builder, "app_wnd_overlay"));
   gtk_window_set_position(GTK_WINDOW(appWidgetsT->main_wnd), GTK_WIN_POS_CENTER);

   appWidgetsT->w_msg_out_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_out_textview"));
   appWidgetsT->w_say_something_entry = GTK_WIDGET(gtk_builder_get_object(builder, "say_something_entry"));


   appWidgetsT->w_sandbox_content = GTK_WIDGET(gtk_builder_get_object(builder, "sandbox_content"));
   AppModel *app_model = app_model_new();
   appWidgetsT->mode_selector = app_mode_selector_new(app_model);
   gtk_box_pack_start(GTK_BOX(appWidgetsT->w_sandbox_content), GTK_WIDGET(appWidgetsT->mode_selector), TRUE, TRUE, 0);
   gtk_widget_show(appWidgetsT->w_sandbox_content);

   appWidgetsT->app_gutter = GTK_WIDGET(gtk_builder_get_object(builder, "app_gutter"));
   appWidgetsT->msg_out = log_terminal_new();
   gtk_box_pack_start(GTK_BOX(appWidgetsT->app_gutter), GTK_WIDGET(appWidgetsT->msg_out), TRUE, TRUE, 0);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}
