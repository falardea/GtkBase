/**
 * Created by french on 3/23/25.
 * @brief
 */
#include <glib-object.h>
#include <ctype.h>
#include "run_model.h"
#include "views/pod_support/halo_shared_variables.h"
#include "views/pod_support/logs.h"
#include "views/pod_support/utils.h"
#include "views/pod_support/recovery.h"

#include "utils/logging.h"

#define RUN_IDENTIFIER_FILE_PATH          ROOT_RECOVERY_DIR "/output_time_string.txt"
#define PREPARATION_START_TIME_FILE_PATH  ROOT_RECOVERY_DIR "/preparation_start_time.txt"

#define RUN_MODEL_RUN_ID_STRING_SZ (128)

struct _RunModel
{
   GObject  parent_object;

   gchar    *run_identifier;
   gchar    *run_directory;
   time_t   preparation_start_time;
   gboolean maintenance_enabled;

   RUN_MODEL_PHASES  run_phase;
   gboolean          albumin_loaded;
};

G_DEFINE_TYPE( RunModel, run_model, G_TYPE_OBJECT )

enum
{
   PROP_0 = 0, // Reserved for GObject
   RUN_MODEL_PROP_RUN_PHASE,
   RUN_MODEL_PROP_ALBUMIN_LOADED,
   RUN_MODEL_N_PROPERTIES
};

enum
{
   RUN_MODEL_SIGNAL_ALBUMIN_LOADED,
   RUN_MODEL_N_SIGNALS
};

void (* run_model_listener_response) (RunModel *model, gboolean loaded);

char *run_model_reformat_run_identifier(char *dest, const char *src, size_t len);

static void run_model_finalize( GObject *g_object )
{
   RunModel *self = RUN_MODEL(g_object);
   g_free(self->run_identifier);
   g_free(self->run_directory);
   G_OBJECT_CLASS (run_model_parent_class)->finalize (g_object);
}

static void run_model_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   RunModel *model = RUN_MODEL( object );

   switch( prop_id ) {
      case RUN_MODEL_PROP_RUN_PHASE:
         run_model_set_run_phase( model, g_value_get_uint( value ) );
         break;
      case RUN_MODEL_PROP_ALBUMIN_LOADED:
         run_model_set_albumin_loaded(model, g_value_get_boolean( value ) );
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
      case RUN_MODEL_PROP_ALBUMIN_LOADED:
         g_value_set_boolean( value , run_model_get_albumin_loaded( model ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static guint      run_model_albumin_loaded_sig[RUN_MODEL_N_SIGNALS] = {0, };
static GParamSpec *model_properties[RUN_MODEL_N_PROPERTIES] = {NULL, };

static void run_model_class_init( RunModelClass *klass )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );

   gobject_class->finalize = run_model_finalize;

   gobject_class->get_property = run_model_get_property;
   gobject_class->set_property = run_model_set_property;

   model_properties[RUN_MODEL_PROP_RUN_PHASE] = g_param_spec_uint("run-phase", "Run Phase", "The current run phase of the model sees itself in",
                                                                  RM_NO_RUN, N_RM_PHASES-1, RM_NO_RUN, G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, RUN_MODEL_N_PROPERTIES, model_properties);

   run_model_albumin_loaded_sig[RUN_MODEL_SIGNAL_ALBUMIN_LOADED] = g_signal_new_class_handler("albumin-loaded",
                                                                                              G_TYPE_FROM_CLASS(klass),
                                                                                              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                                                                              (GCallback)run_model_listener_response,
                                                                                              NULL,
                                                                                              NULL,
                                                                                              g_cclosure_marshal_VOID__BOOLEAN,
                                                                                              G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
}

/////////////////// INSTANCE //////////////////////////////
static void run_model_init( __attribute__((unused)) RunModel *self )
{
}

RunModel *run_model_new( )
{
   RunModel *model;

   model = g_object_new(RUN_TYPE_MODEL,
                        "run-phase", RM_NO_RUN,
                        NULL);

   model->run_identifier = g_strndup("\0", RUN_MODEL_RUN_ID_STRING_SZ);
   model->run_directory = g_strndup("\0", RUN_MODEL_RUN_ID_STRING_SZ);
   model->preparation_start_time = (time_t) -1;

   model->maintenance_enabled = FALSE;

   return model;
}

RUN_MODEL_PHASES run_model_get_run_phase( RunModel *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), FALSE );
   return self->run_phase;
}
void run_model_set_run_phase( RunModel *self, RUN_MODEL_PHASES phase )
{
   g_return_if_fail( RUN_IS_MODEL( self ) );
   self->run_phase = phase;
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_RUN_PHASE]);
}

gchar *run_model_get_run_identifier(RunModel *self)
{
   return self->run_identifier;
}
void run_model_set_run_identifier(RunModel *self, const char *run_id)
{
   // Save the time the run started
   self->preparation_start_time = time(NULL);
   saveLongToFile(self->preparation_start_time, PREPARATION_START_TIME_FILE_PATH);

   // Convert it to a time struct
   struct tm local;
   if (localtime_r(&(self->preparation_start_time), &local) == NULL)
   {
      g_perror("%d", errno);
      return;
   }

   // Convert it to a string
   char timeDateStr[60] = { 0 };
   if (strftime(timeDateStr, sizeof(timeDateStr), "%Y-%m-%d_T%H-%M-%S-%Z", &local) == 0)
   {
      // Error: this leaves the string in an unspecified state, so set the first byte to null for safety
      logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
      timeDateStr[0] = '\0';
      return;
   }

   // Capitalize/sanitize the UNOS ID
   char capitalizedUnosId[64];
   run_model_reformat_run_identifier(capitalizedUnosId, run_id, sizeof(capitalizedUnosId));

   // The run identifier string is "<UNOS ID>__<Start date and time>"
   snprintf(self->run_identifier, RUN_MODEL_RUN_ID_STRING_SZ-1, "%s__%s", capitalizedUnosId, timeDateStr);

   // Save the timestamp to a file
   saveStringToFile(self->run_identifier, RUN_IDENTIFIER_FILE_PATH);

   // Create runs directory (probably exists already)
   if (!createDirIfNotFound(RUNS_DIR))
   {
      g_perror("Failed to create " RUNS_DIR);
   }

   // Create runid directory (probably does not exist yet)
   snprintf(self->run_directory, RUN_MODEL_RUN_ID_STRING_SZ-1, RUNS_DIR "/%s", self->run_identifier);
   if (!createDirIfNotFound(self->run_directory))
   {
      g_perror("Failed to create %s", self->run_directory);
   }

   logging_llprintf(LOGLEVEL_DEBUG, "Run started: %s", self->run_directory);
}
char *run_model_reformat_run_identifier(char *dest, const char *src, size_t len)
{
   // This is just strncpy, capitalizing as it goes and replacing
   // non-alphanumeric characters (e.g. spaces) with underscores
   for (size_t i = 0; i < len - 1; ++i)
   {
      if (isalnum((unsigned char) src[i]))
      {
         dest[i] = (char) toupper((unsigned char) src[i]);
      }
      else if (src[i] == '\0')
      {
         dest[i] = '\0';
         break;
      }
      else
      {
         dest[i] = '_';
      }
   }
   dest[len - 1] = '\0';

   return (dest);
}

gboolean run_model_get_maintenance_enabled(RunModel *self)
{
   g_return_val_if_fail(RUN_IS_MODEL(self), FALSE);
   return self->maintenance_enabled;
}
void run_model_set_maintenance_enabled(RunModel *self, gboolean enabled)
{
   g_return_if_fail(RUN_IS_MODEL(self));
   self->maintenance_enabled = enabled;
}

gboolean run_model_get_albumin_loaded(RunModel *self)
{
   g_return_val_if_fail(RUN_IS_MODEL(self), FALSE);
   return self->albumin_loaded;
}
void run_model_set_albumin_loaded(RunModel *self, gboolean loaded)
{
   g_return_if_fail(RUN_IS_MODEL(self));
   self->albumin_loaded = loaded;
   g_object_notify_by_pspec(G_OBJECT(self), model_properties[RUN_MODEL_PROP_ALBUMIN_LOADED]);
}
