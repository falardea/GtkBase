/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef RUN_MODEL_H__
#define RUN_MODEL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define RUN_TYPE_MODEL              ( run_model_get_type( ) )

G_DECLARE_FINAL_TYPE(RunModel, run_model, RUN, MODEL, GObject)

typedef enum
{
   RM_NO_RUN = 0,
   RM_PHASE_1 = 1,
   RM_PHASE_2 = 2,
   RM_FAILED,
   RM_CANCELLED,
   // Grok suggested phases, which kinda make sense,
   // especially if we're going to have a GError and
   // a cancel
   N_RM_PHASES
} RUN_MODEL_PHASES;

/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new();

RUN_MODEL_PHASES run_model_get_run_phase( RunModel *self );
void run_model_set_run_phase( RunModel *self, RUN_MODEL_PHASES phase );

// Grok suggestion for data we can embed/pack into the model
void run_model_set_data(RunModel *self, const gchar *key, gpointer data, GDestroyNotify destroy);
gpointer run_model_get_data(RunModel *self, const gchar *key);

// Grok also suggested added a GError to the GObject, I thought it was a good suggestion
void run_model_set_error(RunModel *self, GError *error);
GError *run_model_get_error(RunModel *self);

G_END_DECLS
#endif  // RUN_MODEL_H__
