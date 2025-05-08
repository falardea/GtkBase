/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include "app_model.h"
#include "utils/logging.h"

struct _AppModel
{
   GObject           super;
   APP_RUN_MODE      run_mode;
};

G_DEFINE_TYPE( AppModel, app_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   APP_MODEL_PROP_RUN_MODE,
   APP_MODEL_N_PROPERTIES
};

enum
{
   APP_MODEL_SIGNAL_MODE_CHANGE,
   APP_MODEL_N_SIGNALS
};

void (* app_model_listener_response) (AppModel *model, APP_RUN_MODE mode);

static void app_model_finalize( GObject *self )
{
   G_OBJECT_CLASS (app_model_parent_class)->finalize (self);
}

static void app_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   AppModel *model = APP_MODEL( object );

   switch( prop_id ) {
      case APP_MODEL_PROP_RUN_MODE:
         app_model_set_run_mode( model, g_value_get_uint( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void app_model_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   AppModel *model = APP_MODEL( object );

   switch( prop_id ) {
      case APP_MODEL_PROP_RUN_MODE:
         g_value_set_uint( value, app_model_get_run_mode( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static guint      app_model_mode_change_sig[APP_MODEL_N_SIGNALS] = {0, };
static GParamSpec *model_properties[APP_MODEL_N_PROPERTIES] = {NULL, };

static void app_model_class_init( AppModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = app_model_finalize;

   gobject_class->get_property = app_model_get_property;
   gobject_class->set_property = app_model_set_property;

   model_properties[APP_MODEL_PROP_RUN_MODE] = g_param_spec_uint("run-mode",
                                                          "App Run Mode",
                                                          "The app run mode of the model",
                                                          RUN_MODE_NOT_SET, N_APP_RUN_MODES-1, RUN_MODE_NOT_SET,
                                                          G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, APP_MODEL_N_PROPERTIES, model_properties);

   app_model_mode_change_sig[APP_MODEL_SIGNAL_MODE_CHANGE] = g_signal_new_class_handler("mode-changed",
                                                                                        G_TYPE_FROM_CLASS(klass),
                                                                                        G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                                                                        (GCallback)app_model_listener_response,
                                                                                        NULL,
                                                                                        NULL,
                                                                                        g_cclosure_marshal_VOID__INT,
                                                                                        G_TYPE_NONE, 1, G_TYPE_INT);
}

static void app_model_init(AppModel *self)
{
   self->run_mode = RUN_MODE_NOT_SET;
}

AppModel *app_model_new()
{
   AppModel *model;
   model = g_object_new(APP_TYPE_MODEL,
                        "run-mode", RUN_MODE_NOT_SET,
                        NULL);
   return model;
}

APP_RUN_MODE app_model_get_run_mode( AppModel *self )
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   g_return_val_if_fail( APP_IS_MODEL( self ), RUN_MODE_NOT_SET);
   return self->run_mode;
}

void app_model_set_run_mode( AppModel *self, APP_RUN_MODE mode )
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   g_return_if_fail( APP_IS_MODEL( self ) );
   g_return_if_fail(mode < N_APP_RUN_MODES);
   self->run_mode = mode;

   g_object_notify_by_pspec(G_OBJECT(self), model_properties[APP_MODEL_PROP_RUN_MODE]);
   g_signal_emit(G_OBJECT(self), app_model_mode_change_sig[APP_MODEL_SIGNAL_MODE_CHANGE], 0, self->run_mode);
}
