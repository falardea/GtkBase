/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef COMMAND_STEP_H__
#define COMMAND_STEP_H__
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "views/setup_wizard/models/run_model.h"
#include "views/setup_wizard/setup_context_loader.h"
G_BEGIN_DECLS

#define COMMAND_TYPE_STEP (command_step_get_type())

G_DECLARE_FINAL_TYPE(CommandStep, command_step, COMMAND, STEP, GtkBox)

CommandStep *command_step_new(const gchar *command_description,
                              RunModel *model,
                              RunModelCommandFn_T run_model_command_fn,
                              gboolean enable,
                              StepExecutable *next,
                              SetupContextLoader *loader);

void command_step_execute(StepExecutable *exec_self, RunModel *model);
StepExecutable *command_step_get_next(StepExecutable *exec_self, RunModel *model);

G_END_DECLS
#endif  // COMMAND_STEP_H__
