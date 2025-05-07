/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef PENDING_STEP_H__
#define PENDING_STEP_H__
#include <gtk/gtk.h>
#include "views/setup_wizard/models/run_model.h"

G_BEGIN_DECLS // __cplusplus guard

#define PENDING_TYPE_STEP (pending_step_get_type())

G_DECLARE_FINAL_TYPE(PendingStep, pending_step, PENDING, STEP, GtkBox)

PendingStep *pending_step_new(const gchar *pending_description, RunModel *model,
                              const gchar *bound_model_prop);

void pending_step_set_process_completed(PendingStep *self, gboolean running);
gboolean pending_step_get_process_completed(PendingStep *self);

G_END_DECLS
#endif  // PENDING_STEP_H__
