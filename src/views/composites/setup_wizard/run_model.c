/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "utils/logging.h"
#include "run_model.h"


struct _RunModel
{
   GObject           parent_object;
   RUN_MODEL_PHASES  run_phase;
   GHashTable        *data;
   GError            *error;
};

G_DEFINE_TYPE( RunModel, run_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   RUN_MODEL_PROP_RUN_PHASE,
   RUN_MODEL_N_PROPERTIES
};

static void run_model_finalize( GObject *self )
{
   RunModel *model = RUN_MODEL(self);
   g_hash_table_destroy(model->data);
   if(model->error)
   {
      g_error_free(model->error);
   }
   G_OBJECT_CLASS (run_model_parent_class)->finalize (self);
}

static void run_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_PHASE:
         run_model_set_run_phase( model, g_value_get_uint( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void run_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_PHASE:
         g_value_set_uint( value, run_model_get_run_phase( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[RUN_MODEL_N_PROPERTIES] = {NULL, };

static void run_model_class_init( RunModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = run_model_finalize;

   gobject_class->get_property = run_model_get_property;
   gobject_class->set_property = run_model_set_property;

   model_properties[RUN_MODEL_PROP_RUN_PHASE] = g_param_spec_uint("run-phase",
                                                          "Run Phase",
                                                          "The current run phase of the model sees itself in",
                                                          RM_NO_RUN, N_RM_PHASES-1, RM_NO_RUN,
                                                          G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, RUN_MODEL_N_PROPERTIES, model_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void run_model_init(RunModel *self)
{
   self->run_phase = RM_NO_RUN;
   self->data = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
   self->error = NULL;
}

RunModel *run_model_new( )
{
   RunModel *model;
   model = g_object_new(RUN_TYPE_MODEL,
                        "run-phase", RM_NO_RUN,
                        NULL);
   return model;
}

RUN_MODEL_PHASES run_model_get_run_phase( RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), RM_NO_RUN);
   return self->run_phase;
}

void run_model_set_run_phase( RunModel *self, RUN_MODEL_PHASES phase )
{
   g_return_if_fail( RUN_IS_MODEL( self ) );
   g_return_if_fail(phase < N_RM_PHASES);
   self->run_phase = phase;
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_RUN_PHASE]);
}

void run_model_set_data(RunModel *self, const gchar *key, gpointer data, GDestroyNotify destroy)
{
   g_return_if_fail(RUN_IS_MODEL(self));
   g_hash_table_insert(self->data, g_strdup(key), data);
   // TODO: Why is this here?
   // The whole hash table is from Grok, and I'm not quite sure why this "destroy" is here
   if (destroy)
   {
      g_hash_table_replace(self->data, g_strdup(key), data);
   }
}

gpointer run_model_get_data(RunModel *self, const gchar *key)
{
   g_return_val_if_fail(RUN_IS_MODEL(self), NULL);
   return g_hash_table_lookup(self->data, key);
}

void run_model_set_error(RunModel *self, GError *error)
{
   g_return_if_fail(RUN_IS_MODEL(self));
   if(self->error)
   {
      g_error_free(self->error);
   }
   self->error = error;
}

GError *run_model_get_error(RunModel *self)
{
   g_return_val_if_fail(RUN_IS_MODEL(self), NULL);
   return self->error;
}