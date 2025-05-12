/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef RUN_MODEL_H__
#define RUN_MODEL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define RUN_TYPE_MODEL              ( run_model_get_type( ) )

G_DECLARE_FINAL_TYPE(RunModel, run_model, RUN, MODEL, GObject)

#define RUN_MODEL_MODE_CHANGE_PROP_STR          "mode-changed"
#define RUN_MODEL_MODE_CHANGE_SIGNAL_STR        "run-mode-changed"
#define RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR    "step-changed"
#define RUN_MODEL_SETUP_STEP_CHANGE_SIGNAL_STR  "setup-step-changed"

typedef enum
{
   RUN_MODE_NOT_SET = 0,
   RUN_MODE_RESTORE,
   RUN_MODE_STANDARD,
   RUN_MODE_TEST,
   N_RUM_MODEL_MODES
} RUN_MODEL_MODE;

typedef enum
{
   RUN_SETUP_UNINITIALIZED = 0,
   RUN_SETUP_MODE_SELECTED,
   RUN_SETUP_MEMCHECK_COMPLETE,
   // ...
   RUN_SETUP_INTERMEDIATE_STEPS,
   // ...
   RUN_SETUP_COMPLETE,
   RUN_SETUP_FAILED,
   RUN_SETUP_N_STEPS
} RUN_SETUP_STEPS;

typedef void (*RunModelSetterCallback_T)(RunModel *self, gpointer user_data);

/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new();

RUN_MODEL_MODE run_model_get_run_mode(RunModel *self );
void run_model_set_run_mode(RunModel *self, RUN_MODEL_MODE mode );

RUN_SETUP_STEPS run_model_get_last_completed_step(RunModel *self );
void run_model_set_last_completed_step(RunModel *self, RUN_SETUP_STEPS step );

gchar *run_model_get_run_description(RunModel *self);
void run_model_set_run_description(RunModel *self, const gchar *run_description);

gboolean run_model_get_fluid_loaded(RunModel *self);
void run_model_set_fluid_loaded(RunModel *self, gboolean loaded);

gboolean run_model_get_low_memory(RunModel *self);
void run_model_set_low_memory(RunModel *self, gboolean is_low);
G_END_DECLS
#endif  // RUN_MODEL_H__
