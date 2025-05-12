/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "run_model.h"
#include "utils/logging.h"

typedef struct
{
   gboolean keep_alive;
}RunModelPrivate;

struct _RunModel
{
   GObject           super;
   RUN_MODEL_MODE    run_mode;
   RUN_SETUP_STEPS  last_completed_phase;

   gchar             *run_description;
};

G_DEFINE_TYPE_WITH_PRIVATE( RunModel, run_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   RUN_MODEL_PROP_RUN_MODE,
   RUN_MODEL_PROP_SETUP_STEP_CHANGE,
   RUN_MODEL_N_PROPERTIES
};

enum
{
   RUN_MODEL_SIGNAL_MODE_CHANGE,
   RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE,
   RUN_MODEL_N_SIGNALS
};

void (* run_model_listener_response) (RunModel *model, RUN_MODEL_MODE mode);
void (* run_step_listener_response) (RunModel *model, RUN_SETUP_STEPS step);

static void run_model_finalize( GObject *g_obj )
{
   RunModel *self = RUN_MODEL(g_obj);
   if(self->run_description)
   {
      g_free(self->run_description);
   }
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);


   G_OBJECT_CLASS (run_model_parent_class)->finalize (g_obj);
}

static void run_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_MODE:
         run_model_set_run_mode( model, g_value_get_uint( value ) );
         break;
      case RUN_MODEL_PROP_SETUP_STEP_CHANGE:
         run_model_set_last_completed_step( model, g_value_get_uint( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void run_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_MODE:
         g_value_set_uint( value, run_model_get_run_mode( model ) );
         break;
      case RUN_MODEL_PROP_SETUP_STEP_CHANGE:
         g_value_set_uint( value, run_model_get_last_completed_step( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static guint      run_model_sigs[RUN_MODEL_N_SIGNALS] = {0, };
static GParamSpec *model_properties[RUN_MODEL_N_PROPERTIES] = {NULL, };

static void run_model_class_init( RunModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = run_model_finalize;

   gobject_class->get_property = run_model_get_property;
   gobject_class->set_property = run_model_set_property;

   model_properties[RUN_MODEL_PROP_RUN_MODE] = g_param_spec_uint(RUN_MODEL_MODE_CHANGE_PROP_STR,
                                                                 RUN_MODEL_MODE_CHANGE_PROP_STR,
                                                                 RUN_MODEL_MODE_CHANGE_PROP_STR,
                                                                 RUN_MODE_NOT_SET, N_RUM_MODEL_MODES-1, RUN_MODE_NOT_SET, G_PARAM_READWRITE );
   model_properties[RUN_MODEL_PROP_SETUP_STEP_CHANGE] = g_param_spec_uint(RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR,
                                                                          RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR,
                                                                          RUN_MODEL_SETUP_STEP_CHANGE_PROP_STR,
                                                                          RUN_SETUP_UNINITIALIZED, RUN_SETUP_N_STEPS-1, RUN_SETUP_UNINITIALIZED, G_PARAM_READWRITE );


   g_object_class_install_properties( gobject_class, RUN_MODEL_N_PROPERTIES, model_properties);

   run_model_sigs[RUN_MODEL_SIGNAL_MODE_CHANGE] = g_signal_new_class_handler(RUN_MODEL_MODE_CHANGE_SIGNAL_STR, G_TYPE_FROM_CLASS(klass),
                                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, (GCallback)run_model_listener_response,
                                                                                        NULL, NULL, g_cclosure_marshal_VOID__INT, G_TYPE_NONE, 1, G_TYPE_INT);
   run_model_sigs[RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE] = g_signal_new_class_handler(RUN_MODEL_SETUP_STEP_CHANGE_SIGNAL_STR, G_TYPE_FROM_CLASS(klass),
                                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, (GCallback)run_step_listener_response,
                                                                                        NULL, NULL, g_cclosure_marshal_VOID__INT, G_TYPE_NONE, 1, G_TYPE_INT);
}

static void run_model_init(RunModel *self)
{
   RunModelPrivate *priv = run_model_get_instance_private(self);
   self->run_mode = RUN_MODE_NOT_SET;
   self->last_completed_phase = RUN_SETUP_UNINITIALIZED;
}

RunModel *run_model_new()
{
   RunModel *model;
   model = g_object_new(RUN_TYPE_MODEL,
                        RUN_MODEL_MODE_CHANGE_PROP_STR, RUN_MODE_NOT_SET,
                        NULL);
   return model;
}

RUN_MODEL_MODE run_model_get_run_mode( RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RUN_MODE_NOT_SET);
   return self->run_mode;
}
void run_model_set_run_mode( RunModel *self, RUN_MODEL_MODE mode )
{
   self->run_mode = mode;
   logging_llprintf(LOGLEVEL_DEBUG, "%s: mode=%d", __func__, self->run_mode);
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_RUN_MODE]);
   g_signal_emit(G_OBJECT(self), run_model_sigs[RUN_MODEL_SIGNAL_MODE_CHANGE], 0, self->run_mode);
}

RUN_SETUP_STEPS run_model_get_last_completed_step(RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RUN_SETUP_FAILED);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   return self->last_completed_phase;
}
void run_model_set_last_completed_step(RunModel *self, RUN_SETUP_STEPS step )
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   // ----------------------
   self->last_completed_phase = step;
   // ----------------------
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_SETUP_STEP_CHANGE]);
   g_signal_emit(G_OBJECT(self), run_model_sigs[RUN_MODEL_SIGNAL_SETUP_STEP_CHANGE], 0, self->last_completed_phase);
}

gchar *run_model_get_run_description(RunModel *self)
{
   return g_strdup(self->run_description);
}
void run_model_set_run_description(RunModel *self, const gchar *run_description)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   self->run_description = g_strdup(run_description);
}
