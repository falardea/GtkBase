/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SETUP_RUNNER_H__
#define SETUP_RUNNER_H__
#include <gtk/gtk.h>
#include "step_executable_interface.h"

G_BEGIN_DECLS

#define SETUP_TYPE_RUNNER (setup_runner_get_type())

G_DECLARE_FINAL_TYPE(SetupRunner, setup_runner, SETUP, RUNNER, GtkBox)

SetupRunner *setup_runner_new(StepExecutable *parent_setup,
                              ExecutableCallback_T on_setup_complete,
                              gpointer callback_user_data);

// The execute function needs to be exposed in order to inject it into children
void setup_runner_execute(StepExecutable *self, RunModel *run_model);
void setup_runner_add_child(SetupRunner *self, StepExecutable *child);

G_END_DECLS
#endif  // SETUP_RUNNER_H__
