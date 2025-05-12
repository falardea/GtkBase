/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "app_mode_selector.h"
#include "mode_prompt.h"
#include "sw_mvc/run_viewer.h"
#include "utils/logging.h"

typedef struct {
   RunViewer         *run_viewer;
   RunModel          *model;
} AppModeSelectorPrivate;

struct _AppModeSelector
{
   GtkBox         super;
   GtkStack       *page_stack;
   GtkBox         *run_page;
   GtkButtonBox   *mode_select_page;
   GtkBox         *config_page;

   GtkButton      *btn_start_new_run;
   GtkButton      *btn_config_page;
   GtkButton      *btn_config_back;

   GtkOverlay     *popup_container;
};

G_DEFINE_TYPE_WITH_PRIVATE(AppModeSelector, app_mode_selector, GTK_TYPE_BOX)

static void app_mode_selector_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(APP_IS_MODE_SELECTOR(g_object));
   // AppModeSelector *self = APP_MODE_SELECTOR(g_object);
   // AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   // if (priv->setup_context != NULL)
   // {
   //    gtk_widget_destroy(GTK_WIDGET(priv->setup_context));
   // }

   G_OBJECT_CLASS(app_mode_selector_parent_class)->finalize(g_object);
}

gboolean app_mode_selector_load_context(AppModeSelector *self)
{
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->run_page));

   return G_SOURCE_REMOVE;
}
gboolean standard_validation_callback(AppModeSelector *self, gchar *description_to_validate, gpointer user_data)
{
   RunModel *model = RUN_MODEL(user_data);
   // AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   if (strlen(description_to_validate) > 0)
   {
      run_model_set_run_mode(model, RUN_MODE_STANDARD);
      run_model_set_run_description(model, description_to_validate);
      g_idle_add((GSourceFunc)app_mode_selector_load_context, self);

      return TRUE;
   }
   return FALSE;
}
gboolean service_validation_callback(AppModeSelector *self, gchar *password_to_validate, gchar *description, gpointer user_data)
{
   RunModel *model = RUN_MODEL(user_data);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   if (strlen(password_to_validate) > 0 && strcmp(password_to_validate, "Abracadabra") == 0) {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, password_to_validate);

      run_model_set_run_mode(model, RUN_MODE_TEST);
      run_model_set_run_description(model, (description != NULL) ? description : "SERVICE");
      g_idle_add((GSourceFunc)app_mode_selector_load_context, self);

      return TRUE;
   }
   return FALSE;
}

void on_btn_start_new_run_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);

   GtkWidget *popup = mode_prompt_new(self, (RunValidationCallback_T)standard_validation_callback,
                                      (ServiceValidationCallback_T)service_validation_callback,
                                      priv->model);
   gtk_overlay_add_overlay(GTK_OVERLAY(self->popup_container), popup);
   gtk_widget_show_all(popup);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

}
void on_btn_config_page_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->config_page));
}
void on_btn_config_back_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AppModeSelector *self = APP_MODE_SELECTOR(user_data);
   gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->mode_select_page));
}

static void app_mode_selector_class_init(AppModeSelectorClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = app_mode_selector_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/app_mode_selector");
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, page_stack);

   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, mode_select_page);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_start_new_run);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_config_page);

   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, config_page);
   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, btn_config_back);

   gtk_widget_class_bind_template_child(widget_class, AppModeSelector, run_page);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_new_run_clicked", (GCallback)on_btn_start_new_run_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_config_page_clicked", (GCallback)on_btn_config_page_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_config_back_clicked", (GCallback)on_btn_config_back_clicked);
}
static void app_mode_selector_init(AppModeSelector *self)
{
   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);
   gtk_widget_init_template(GTK_WIDGET(self));

   priv->run_viewer = run_viewer_new(priv->model);
   gtk_box_pack_start(self->run_page, GTK_WIDGET(priv->run_viewer), TRUE, TRUE, 0);
   priv->model = NULL;
}
AppModeSelector *app_mode_selector_new(RunModel *model, GtkOverlay *popup_container)
{
   g_return_val_if_fail(model != NULL, NULL);
   AppModeSelector *self;
   self = g_object_new(APP_TYPE_MODE_SELECTOR, NULL);

   AppModeSelectorPrivate *priv = app_mode_selector_get_instance_private(self);
   priv->model = model;

   self->popup_container = popup_container;

   return self;
}
