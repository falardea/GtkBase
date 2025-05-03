/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "utils/logging.h"
#include "alarm_model.h"


struct _AlarmModel
{
   GObject           super;
   ALARM_MODEL_LEVEL alarm_level;
};

G_DEFINE_TYPE( AlarmModel, alarm_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   ALARM_MODEL_PROP_ALARM_LEVEL,
   ALARM_MODEL_N_PROPERTIES
};

static void alarm_model_finalize( GObject *self );

static void alarm_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   AlarmModel *model = ALARM_MODEL( object );

   switch( prop_id ) {
      case ALARM_MODEL_PROP_ALARM_LEVEL:
         alarm_model_set_alarm_level( model, g_value_get_uint( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void alarm_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   AlarmModel *model = ALARM_MODEL( object );

   switch( prop_id ) {
      case ALARM_MODEL_PROP_ALARM_LEVEL:
         g_value_set_uint( value, alarm_model_get_alarm_level( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *model_properties[ALARM_MODEL_N_PROPERTIES] = {NULL, };

static void alarm_model_class_init( AlarmModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = alarm_model_finalize;

   gobject_class->get_property = alarm_model_get_property;
   gobject_class->set_property = alarm_model_set_property;

   model_properties[ALARM_MODEL_PROP_ALARM_LEVEL] = g_param_spec_uint("alarm-level",
                                                          "Alarm Level",
                                                          "The alarm level of the model",
                                                          AM_NO_ALARM, AM_BASIC_HIGH_ALARM, AM_NO_ALARM,
                                                          G_PARAM_READWRITE );

   g_object_class_install_properties( gobject_class, ALARM_MODEL_N_PROPERTIES, model_properties);
}

/////////////////// INSTANCE //////////////////////////////
static void alarm_model_init( __attribute__((unused)) AlarmModel *self )
{
}

static void alarm_model_finalize( GObject *self )
{
   G_OBJECT_CLASS (alarm_model_parent_class)->finalize (self);
}

AlarmModel *alarm_model_new( )
{
   AlarmModel *model;

   // GValue init_alarm_val = G_VALUE_INIT;
   // g_value_init(&init_alarm_val, G_TYPE_UINT);
   // g_value_set_uint(&init_alarm_val, AM_NO_ALARM);

   model = g_object_new(ALARM_TYPE_MODEL,
                        "alarm-level", AM_BASIC_HIGH_ALARM,
                        NULL);

   // g_value_unset(&init_alarm_val);

   return model;
}

ALARM_MODEL_LEVEL alarm_model_get_alarm_level( AlarmModel *self )
{
   g_return_val_if_fail( ALARM_IS_MODEL( self ), FALSE );
   return self->alarm_level;
}
void alarm_model_set_alarm_level( AlarmModel *self, ALARM_MODEL_LEVEL level )
{
   g_return_if_fail( ALARM_IS_MODEL( self ) );
   self->alarm_level = level;
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[ALARM_MODEL_PROP_ALARM_LEVEL]);
}
