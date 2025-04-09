/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "ui_builder.h"
#include "utils/logging.h"
#include "composites/alarms_content/alarms_content.h"
#include "composites/setup_wizard/sequence_runner.h"

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;

   app_widget_ref_struct *appWidgetsT = g_slice_new(app_widget_ref_struct);

   builder = gtk_builder_new();

   if (gtk_builder_add_from_resource(builder, "/resource_path/this_app_name.glade", NULL) == 0) {
      logging_llprintf(LOGLEVEL_ERROR, "failed to load glade resource");
      return NULL;
   }

   appWidgetsT->w_msg_out_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_out_textview"));
   appWidgetsT->w_say_something_entry = GTK_WIDGET(gtk_builder_get_object(builder, "say_something_entry"));

   appWidgetsT->w_app_content_box = GTK_WIDGET(gtk_builder_get_object(builder, "app_content_box"));

   appWidgetsT->w_alarms_content_root = alarms_content_new();
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_app_content_box), GTK_WIDGET(appWidgetsT->w_alarms_content_root), TRUE, TRUE,0);

   appWidgetsT->w_sequence_runner = sequence_runner_new();
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_app_content_box), GTK_WIDGET(appWidgetsT->w_sequence_runner), TRUE, TRUE,0);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}
