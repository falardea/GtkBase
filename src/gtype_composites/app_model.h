/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef APP_MODEL_H__
#define APP_MODEL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define APP_TYPE_MODEL              ( app_model_get_type( ) )

G_DECLARE_FINAL_TYPE(AppModel, app_model, APP, MODEL, GObject)

#define APP_RUN_MODE_SIGNAL_STR  "run-mode-changed"

typedef enum
{
   RUN_MODE_NOT_SET = 0,
   RUN_MODE_RESTORE,
   RUN_MODE_STANDARD,
   RUN_MODE_TEST,
   N_APP_RUN_MODES
} APP_RUN_MODE;

/* Methods for our newly declared GObject derived GType*/
AppModel *app_model_new();

APP_RUN_MODE app_model_get_run_mode( AppModel *self );
void app_model_set_run_mode( AppModel *self, APP_RUN_MODE mode );

gchar *app_model_get_run_description(AppModel *self);
void app_model_set_run_description(AppModel *self, const gchar *run_description);

G_END_DECLS
#endif  // APP_MODEL_H__
