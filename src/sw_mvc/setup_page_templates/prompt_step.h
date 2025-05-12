/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef PROMPT_STEP_H__
#define PROMPT_STEP_H__
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "views/setup_wizard/interfaces/step_executable_interface.h"
#include "views/setup_wizard/models/run_model.h"
#include "views/setup_wizard/setup_context_loader.h"

G_BEGIN_DECLS

#define PROMPT_TYPE_STEP (prompt_step_get_type())

G_DECLARE_FINAL_TYPE(PromptStep, prompt_step, PROMPT, STEP, GtkBox)

PromptStep *prompt_step_new(const gchar *prompt_description,
                            StepExecutable *next,
                            RunModel *model,
                            SetupContextLoader *loader);

void prompt_step_execute(StepExecutable *exec_self, RunModel *model);
StepExecutable *prompt_step_get_next(StepExecutable *exec_self, RunModel *model);

G_END_DECLS
#endif  // PROMPT_STEP_H__
