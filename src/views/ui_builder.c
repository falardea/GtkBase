/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "ui_builder.h"
#include "utils/logging.h"
#include "composites/alarms_content/alarms_content.h"
#include "composites/setup_wizard/sequence_runner.h"


void on_setup_complete(__attribute__((unused)) StepExecutable *placeholder,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "Setup Complete");
}

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
   appWidgetsT->w_box_sandbox_content = GTK_WIDGET(gtk_builder_get_object(builder, "box_sandbox_content"));

//   appWidgetsT->w_alarms_content_root = alarms_content_new();
//   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_app_content_box), GTK_WIDGET(appWidgetsT->w_alarms_content_root), TRUE, TRUE,0);

   appWidgetsT->w_sequence_runner = sequence_runner_new(on_setup_complete, NULL);
   gtk_box_pack_end(GTK_BOX(appWidgetsT->w_box_sandbox_content), GTK_WIDGET(appWidgetsT->w_sequence_runner), TRUE, TRUE,0);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}


void build_setup_tab(GtkBuilder *builder, app_widgets *widgets)
{
   GET_WIDGET(tab_setup);
   GET_WIDGET(app_content_box);

   widgets->w_setup_runner = setup_runner_new(NULL, on_setup_complete, NULL);
   RunModel *rmodel = run_model_new();
   widgets->g_run_model = rmodel;

   gtk_box_pack_end(GTK_BOX(widgets->w_app_content_box), GTK_WIDGET(widgets->w_setup_runner), TRUE, TRUE,0);

   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_sequence_new("Setup Starting","START",STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));

   SequenceRunner *sr_embed = sequence_runner_new(STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL);
   StepSequence *step_seq = step_sequence_new("Sequence-in-Sequence-1 Starting","START",STEP_EXECUTABLE(sr_embed), setup_runner_execute,NULL);
   StepTimeout *step_object = step_timeout_new("Sequence-in-Sequence-1 Countdown", 1, STEP_EXECUTABLE(sr_embed), setup_runner_execute, NULL);
   StepAcknowledge *step_ack = step_acknowledge_new("Sequence-in-Sequence-1 Ack Step", "NEXT", STEP_EXECUTABLE(sr_embed), setup_runner_execute, NULL);

   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(step_seq));
   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(step_object));
   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(step_ack));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(sr_embed));

   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_timeout_new("Leaf Timeout Step 1", 1, STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_acknowledge_new("Leaf Ack Step 1", "NEXT", STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_timeout_new("Leaf Timeout Step 2", 1, STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_acknowledge_new("Leaf Ack Step 2", "NEXT", STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(step_timeout_new("Leaf Timeout Step 3", 1, STEP_EXECUTABLE(widgets->w_setup_runner), setup_runner_execute, NULL)));

}