/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "utils/logging.h"
#include "alarm_model.h"


struct _AlarmModel
{
   GObject  parent_object;
   gboolean enabled;
   gchar    *model_name;
};

G_DEFINE_TYPE( AlarmModel, alarm_model, G_TYPE_OBJECT )

enum
{
   PROP_0, // Reserved for GObject
   PROP_ENABLED,
   PROP_MODEL_NAME,
   N_ALARM_MODEL_PROPERTIES
};

static void alarm_model_finalize( GObject *self );

static void alarm_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   AlarmModel *model = ALARM_MODEL( object );

   switch( prop_id ) {
      case PROP_MODEL_NAME:
         alarm_model_set_name( model, g_value_get_string( value ) );
         break;
      case PROP_ENABLED:
         alarm_model_set_enabled(model, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void alarm_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   AlarmModel *model = ALARM_MODEL( object );

   switch( prop_id ) {
      case PROP_MODEL_NAME:
         g_value_set_string( value, alarm_model_get_name( model ) );
         break;
      case PROP_ENABLED:
         g_value_set_boolean( value, alarm_model_get_enabled( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[N_ALARM_MODEL_PROPERTIES] = {NULL, };

static void alarm_model_class_init( AlarmModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = alarm_model_finalize;

   gobject_class->get_property = alarm_model_get_property;
   gobject_class->set_property = alarm_model_set_property;

   model_properties[PROP_ENABLED] = g_param_spec_boolean("model-enabled",
                                                          "Device Enabled",
                                                          "Device's enabled state",
                                                          FALSE,
                                                          G_PARAM_READWRITE );
   model_properties[PROP_MODEL_NAME] = g_param_spec_string("model-name",
                                                             "DeviceName",
                                                             "Device's Name",
                                                             NULL,
                                                             G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, N_ALARM_MODEL_PROPERTIES, model_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void alarm_model_init( AlarmModel *self )
{
   self->model_name = g_strdup( "INIT_UNNAMED" );
   self->enabled = FALSE;
   logging_llprintf(LOGLEVEL_DEBUG, "object instance init: model_name = %s", self->model_name);
}

static void alarm_model_finalize( GObject *self )
{
   AlarmModel *thisDevice = ALARM_MODEL(self );
   g_free(thisDevice->model_name);
   logging_llprintf(LOGLEVEL_DEBUG, "object instance finalize");
   G_OBJECT_CLASS (alarm_model_parent_class)->finalize (self);
}

AlarmModel *alarm_model_new( const gchar *name )
{
   AlarmModel *model;

   model = ALARM_MODEL( g_object_new( ALARM_TYPE_MODEL, NULL ) );

   if( name != NULL )
      alarm_model_set_name( model, name );

   return model;
}

gboolean alarm_model_get_enabled( AlarmModel *self )
{
   g_return_val_if_fail( ALARM_IS_MODEL( self ), FALSE );
   return self->enabled;
}
void alarm_model_set_enabled( AlarmModel *self, gboolean enabled )
{
   g_return_if_fail( ALARM_IS_MODEL( self ) );
   self->enabled = enabled;
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[PROP_ENABLED]);
}

gchar *alarm_model_get_name( AlarmModel *self )
{
   g_return_val_if_fail( ALARM_IS_MODEL( self ), NULL );

   return g_strdup( self->model_name );
}
void alarm_model_set_name( AlarmModel *self, const gchar *name )
{
   g_return_if_fail( name );
   g_return_if_fail( ALARM_IS_MODEL( self ) );

   if( self->model_name != NULL )
      g_free( self->model_name );

   self->model_name = g_strdup( name );
}

