/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "app_mode_selector.h"
#include "mode_prompt.h"
#include "utils/logging.h"

typedef struct {
   AppModel *model;
} AppModeSelectorPrivate;

struct _AppModeSelector
{
   GtkBox         super;
   GtkBox         *mode_run_context;
   GtkButtonBox   *mode_select_page;

   GtkButton      *btn_restore_mode;
   GtkButton      *btn_standard_mode;
   GtkButton      *btn_test_mode;
};

G_DEFINE_TYPE_WITH_PRIVATE(AppModeSelector, app_mode_selector, GTK_TYPE_BOX)

static void app_mode_selector_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(APP_IS_MODE_SELECTOR(g_object));
   // AppModeSelector *self = APP_MODE_SELECTOR(g_object);
   // AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   G_OBJECT_CLASS(app_mode_selector_parent_class)->finalize(g_object);
}

void on_btn_restore_mode_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   app_model_set_run_mode(priv->model, RUN_MODE_RESTORE);
}


gboolean validation_callback(gchar *text_to_validate)
{
   app_widgets *wdgts = get_app_widgets_pointer();

   if (strlen(text_to_validate) > 0 && strcmp(text_to_validate, "Abracadabra") == 0)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, text_to_validate);
      return TRUE;
   }
   return FALSE;
}

void on_btn_standard_mode_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   // GtkWidget *popup = mode_prompt_new((ValidateFormCallback_T)validation_callback);
   // gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->app_wnd_overlay), popup);
   // gtk_widget_show_all(popup);

   app_model_set_run_mode(priv->model, RUN_MODE_STANDARD);
}
void on_btn_test_mode_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   app_model_set_run_mode(priv->model, RUN_MODE_TEST);
}

static void app_mode_selector_class_init(AppModeSelectorClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = app_mode_selector_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/app_mode_selector");
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, mode_run_context);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, mode_select_page);

   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_restore_mode);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_standard_mode);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_test_mode);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_restore_mode_clicked", (GCallback)on_btn_restore_mode_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_standard_mode_clicked", (GCallback)on_btn_standard_mode_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_test_mode_clicked", (GCallback)on_btn_test_mode_clicked);
}

static void app_mode_selector_init(AppModeSelector *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);
   gtk_widget_init_template(GTK_WIDGET(self));
   priv->model = NULL;
}

AppModeSelector *app_mode_selector_new(AppModel *model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_val_if_fail(model != NULL, NULL);
   AppModeSelector *self;
   self = g_object_new(APP_TYPE_MODE_SELECTOR, NULL);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);
   priv->model = model;
   return self;
}
