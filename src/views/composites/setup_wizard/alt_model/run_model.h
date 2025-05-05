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
   RM_PHASE_COMPLETE = 10,
   N_RM_PHASES
} RUN_MODEL_PHASES;

/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new();

RUN_MODEL_PHASES run_model_get_run_phase( RunModel *self );
void run_model_set_run_phase( RunModel *self, RUN_MODEL_PHASES phase );

gchar *run_model_get_run_identifier(RunModel *self);
void run_model_set_run_identifier(RunModel *self, const char *run_id);

gboolean run_model_get_maintenance_enabled(RunModel *self);
void run_model_set_maintenance_enabled(RunModel *self, gboolean enabled);

void run_model_set_albumin_loaded(RunModel *self, gboolean loaded);
gboolean run_model_get_albumin_loaded(RunModel *self);

G_END_DECLS
#endif  // RUN_MODEL_H__
