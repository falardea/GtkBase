/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef PENDING_STEP_H__
#define PENDING_STEP_H__
#include <gtk/gtk.h>
#include "views/setup_wizard/models/run_model.h"
#include "views/setup_wizard/interfaces/step_executable_interface.h"
#include "views/setup_wizard/setup_context_loader.h"
G_BEGIN_DECLS // __cplusplus guard

#define PENDING_TYPE_STEP (pending_step_get_type())

G_DECLARE_FINAL_TYPE(PendingStep, pending_step, PENDING, STEP, GtkBox)

PendingStep *pending_step_new(const gchar *pending_description,
                              RunModel *model,
                              const gchar *bound_model_prop,
                              StepExecutable *on_success,
                              StepExecutable *on_failure,
                              SetupContextLoader *loader);

void pending_step_set_process_completed(PendingStep *self, gboolean running);
gboolean pending_step_get_process_completed(PendingStep *self);

void pending_step_execute(StepExecutable *exec_self, RunModel *model);
StepExecutable *pending_step_get_next(StepExecutable *exec_self, RunModel *model);

void pending_step_set_next_on_success(PendingStep *self, StepExecutable *next_on_success);
void pending_step_set_next_on_failure(PendingStep *self, StepExecutable *next_on_failure);

G_END_DECLS
#endif  // PENDING_STEP_H__
