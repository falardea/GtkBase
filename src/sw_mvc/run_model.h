/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef RUN_MODEL_H__
#define RUN_MODEL_H__

#include <gtk/gtk.h>
#include "context_mediator.h"

G_BEGIN_DECLS

typedef struct _RunModel RunModel;
typedef struct _ContextMediator ContextMediator;

#define RUN_TYPE_MODEL              ( run_model_get_type( ) )

G_DECLARE_FINAL_TYPE(RunModel, run_model, RUN, MODEL, GObject)

#define RUN_MODEL_MODE_CHANGE_SIGNAL_STR  "run-mode-changed"

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
} RUN_SETUP_PHASES;

/* Methods for our newly declared GObject derived GType*/
RunModel *run_model_new(GtkLabel *context_label,
                        GtkBox *parent_context);

RUN_MODEL_MODE run_model_get_run_mode(RunModel *self );
void run_model_set_run_mode(RunModel *self, RUN_MODEL_MODE mode );

RUN_SETUP_PHASES run_model_get_last_completed_phase(RunModel *self );
void run_model_set_last_completed_phase(RunModel *self, RUN_SETUP_PHASES phase );

gchar *run_model_get_run_description(RunModel *self);
void run_model_set_run_description(RunModel *self, const gchar *run_description);

ContextMediator *run_model_get_ui_context(RunModel *self);
void run_model_set_ui_context(RunModel *self);

G_END_DECLS
#endif  // RUN_MODEL_H__
