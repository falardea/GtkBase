#ifndef STEP_STRATEGY_H__
#define STEP_STRATEGY_H__
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "step_executable_interface.h"

G_BEGIN_DECLS

#define STEP_TYPE_STRATEGY (step_strategy_get_type())

G_DECLARE_FINAL_TYPE(StepStrategy, step_strategy, STEP, STRATEGY, GObject)

StepStrategy *step_strategy_new(ExecutableConditionCallback_T condition,
                                GtkWidget *default_view);

void step_strategy_add_strategy(StepStrategy *self, StepExecutable *strategy);

G_END_DECLS
#endif  // STEP_STRATEGY_H__
