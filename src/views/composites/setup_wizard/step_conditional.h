#ifndef STEP_CONDITIONAL_H__
#define STEP_CONDITIONAL_H__
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "step_executable_interface.h"

G_BEGIN_DECLS

#define STEP_TYPE_CONDITIONAL (step_conditional_get_type())

G_DECLARE_FINAL_TYPE(StepConditional, step_conditional, STEP, CONDITIONAL, GtkBox)

StepConditional *step_conditional_new(StepExecutable *parent_sequence,
                                      StepExecutable *start_step,
                                      StepExecutable *cond_true_step,
                                      StepExecutable *cond_false_step,
                                      ExecutableConditionCallback_T conditional_callback,
                                      gpointer callback_user_data);

void step_conditional_execute(StepExecutable *self, RunModel *run_model);

G_END_DECLS
#endif  // STEP_CONDITIONAL_H__
