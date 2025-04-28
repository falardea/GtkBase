/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "ui_builder.h"
#include "utils/logging.h"
#include "composites/alarms_content/alarms_content.h"
#include "composites/setup_wizard/setup_runner.h"
#include "composites/setup_wizard/sequence_runner.h"
#include "composites/setup_wizard/step_acknowledge.h"
#include "composites/setup_wizard/step_sequence.h"
#include "composites/setup_wizard/step_timeout.h"
#include "composites/setup_wizard/run_model.h"


void on_setup_complete(__attribute__((unused)) StepExecutable *placeholder,__attribute__((unused)) RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "Setup Complete");
   logging_llprintf(LOGLEVEL_DEBUG, "%s: Run Phase %d", __func__, run_model_get_run_phase(run_model));
}

void on_random_phase_update(__attribute__((unused)) StepExecutable *placeholder,__attribute__((unused)) RunModel *run_model)
{
   run_model_set_run_phase(run_model, RM_PHASE_2);
   logging_llprintf(LOGLEVEL_DEBUG, "%s: Run Phase %d", __func__, run_model_get_run_phase(run_model));
}

void example_external_callback(__attribute__((unused)) StepExecutable *placeholder,__attribute__((unused)) RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, "!!!!!!!!!! Checkpoint External Call !!!!!!!!!!!!!");
}

void build_setup_tab(__attribute__((unused)) GtkBuilder *builder, app_widget_ref_struct *widgets)
{
   RunModel *rmodel = run_model_new();
   widgets->g_run_model = rmodel;

   widgets->w_sequence_runner = sequence_runner_new();
   gtk_box_pack_end(GTK_BOX(widgets->w_app_content_box), GTK_WIDGET(widgets->w_sequence_runner), TRUE, TRUE, 0);

   StepTimeout *timeout_leaf_one = step_timeout_new("Leaf Timeout Step 1", 3, NULL,widgets->g_run_model);

   sequence_runner_add_child(widgets->w_sequence_runner, STEP_EXECUTABLE(timeout_leaf_one));

/*
   RunModel *rmodel = run_model_new();
   widgets->g_run_model = rmodel;
   widgets->w_setup_runner = setup_runner_new(NULL, on_setup_complete, widgets->g_run_model);
   gtk_box_pack_end(GTK_BOX(widgets->w_app_content_box), GTK_WIDGET(widgets->w_setup_runner), TRUE, TRUE,0);
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_sequence_new("Setup Starting","START",STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model)));
   SequenceRunner *sr_embed = sequence_runner_new(STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model);
   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(
         step_sequence_new("Seq-in-Seq-1 Starting","START",STEP_EXECUTABLE(sr_embed), example_external_callback, widgets->g_run_model)));
   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(
         step_timeout_new("Seq-in-Seq-1 Countdown", 1, STEP_EXECUTABLE(sr_embed), NULL, widgets->g_run_model)));
   sequence_runner_add_child(sr_embed, STEP_EXECUTABLE(
         step_acknowledge_new("Seq-in-Seq-1 Ack Step", "NEXT", STEP_EXECUTABLE(sr_embed), NULL, widgets->g_run_model)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(sr_embed));

   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_timeout_new("Leaf Timeout Step 1", 1, STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_acknowledge_new("Leaf Ack Step 1", "NEXT", STEP_EXECUTABLE(widgets->w_setup_runner), on_random_phase_update, widgets->g_run_model)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_timeout_new("Leaf Timeout Step 2", 1, STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_acknowledge_new("Leaf Ack Step 2", "NEXT", STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model)));
   setup_runner_add_child(widgets->w_setup_runner, STEP_EXECUTABLE(
         step_timeout_new("Leaf Timeout Step 3", 1, STEP_EXECUTABLE(widgets->w_setup_runner), NULL, widgets->g_run_model)));
         */
}

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;
   g_type_ensure(STEP_TYPE_TIMEOUT);
   g_type_ensure(STEP_TYPE_ACKNOWLEDGE);
   g_type_ensure(STEP_TYPE_SEQUENCE);

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

   // appWidgetsT->g_run_model = run_model_new();
   // appWidgetsT->w_sequence_runner = sequence_runner_new(NULL, on_setup_complete, appWidgetsT->g_run_model);
   // gtk_box_pack_end(GTK_BOX(appWidgetsT->w_box_sandbox_content), GTK_WIDGET(appWidgetsT->w_sequence_runner), TRUE, TRUE,0);

   build_setup_tab(builder, appWidgetsT);

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}

