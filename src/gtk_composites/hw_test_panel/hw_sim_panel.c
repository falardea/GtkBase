/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "hw_sim_panel.h"
#include "utils/logging.h"

typedef struct
{
   AppModel *model;
}HwSimPanelPrivate;

struct _HwSimPanel
{
   GtkWindow      super;

   GtkBox         *memcheck_response_box;
   GtkBox         *pumping_response_box;
   GtkBox         *heating_response_box;

   GtkButton      *hw_sim_panel_close;
   GtkButton      *btn_memcheck_response;
   GtkButton      *btn_pump_enable_response;
   GtkButton      *btn_heat_enable_response;

   GtkCheckButton *ckbtn_memcheck_failure;
   GtkCheckButton *ckbtn_pump_enable_failure;
   GtkCheckButton *ckbtn_heat_enable_failure;
};

G_DEFINE_TYPE_WITH_PRIVATE(HwSimPanel, hw_sim_panel, GTK_TYPE_WINDOW)

static void hw_sim_panel_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(HW_IS_SIM_PANEL(g_object));
   G_OBJECT_CLASS(hw_sim_panel_parent_class)->finalize(g_object);
}

static void on_hw_sim_panel_close_clicked(GtkWidget *button, gpointer *user_data);
static void on_hw_sim_panel_destroy(GtkWidget *self);
gboolean on_hw_sim_panel_delete_event(GtkWidget *srcWidget, GdkEvent *event, gpointer uData);

static void on_btn_memcheck_response_clicked(GtkButton *button, gpointer *user_data);
static void on_btn_pump_enable_response_clicked(GtkButton *button, gpointer *user_data);
static void on_btn_heat_enable_response_clicked(GtkButton *button, gpointer *user_data);

static void on_ckbtn_memcheck_failure_toggled(GtkToggleButton *button, gpointer *user_data);
static void on_ckbtn_pump_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);
static void on_ckbtn_heat_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);

static void hw_sim_panel_class_init(HwSimPanelClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = hw_sim_panel_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/hw_sim_panel");
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, hw_sim_panel_close);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, memcheck_response_box);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, pumping_response_box);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, heating_response_box);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_memcheck_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_pump_enable_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_heat_enable_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_memcheck_failure);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_heat_enable_failure);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_pump_enable_failure);

   // Might be a bit overkill with all these close/destroy/delete... but we probably still aren't freeing the memory correctly for this
   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_delete_event", (GCallback)on_hw_sim_panel_delete_event);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_destroy", (GCallback)on_hw_sim_panel_destroy);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_close_clicked", (GCallback)on_hw_sim_panel_close_clicked);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_memcheck_response_clicked", (GCallback)on_btn_memcheck_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_pump_enable_response_clicked", (GCallback)on_btn_pump_enable_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_heat_enable_response_clicked", (GCallback)on_btn_heat_enable_response_clicked);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_memcheck_failure_toggled", (GCallback)on_ckbtn_memcheck_failure_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_pump_enable_failure_toggled", (GCallback)on_ckbtn_pump_enable_failure_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_heat_enable_failure_toggled", (GCallback)on_ckbtn_heat_enable_failure_toggled);
}

static void hw_sim_panel_init(HwSimPanel *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(HW_IS_SIM_PANEL(self));
   gtk_widget_init_template(GTK_WIDGET(self));
   HwSimPanelPrivate *priv = hw_sim_panel_get_instance_private(self);
   priv->model = NULL;
}

void hw_sim_panel_rx_mode_change(GObject *source, APP_RUN_MODE sig_val, gpointer user_data)
{
   HwSimPanel *self = HW_SIM_PANEL(user_data);

   logging_llprintf(LOGLEVEL_DEBUG, "%s: %d", __func__, sig_val);

   gtk_widget_set_sensitive(GTK_WIDGET(self->memcheck_response_box), TRUE);
}

HwSimPanel *hw_sim_panel_new(AppModel *model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   HwSimPanel *self;
   self = g_object_new(HW_TYPE_SIM_PANEL, NULL);

   HwSimPanelPrivate *priv = hw_sim_panel_get_instance_private(self);
   priv->model = model;

//   g_object_bind_property(G_OBJECT(model), "run-mode", self, ??, G_BINDING_DEFAULT);

   logging_llprintf(LOGLEVEL_DEBUG, "%s: pre-connect", __func__);

   g_signal_connect (G_OBJECT(priv->model), "mode-changed", G_CALLBACK(hw_sim_panel_rx_mode_change), self);

   return self;
}

/////////////////////
static void on_btn_memcheck_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_btn_pump_enable_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_btn_heat_enable_response_clicked(__attribute__((unused))GtkButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void on_ckbtn_memcheck_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_ckbtn_pump_enable_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_ckbtn_heat_enable_failure_toggled(__attribute__((unused))GtkToggleButton *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void on_hw_sim_panel_close_clicked(GtkWidget *button,__attribute__((unused)) gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_window_close(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
}
static void on_hw_sim_panel_destroy(GtkWidget *self)
{
   gtk_widget_destroyed(self, &self);
}
gboolean on_hw_sim_panel_delete_event(__attribute__((unused)) GtkWidget *srcWidget,__attribute__((unused)) GdkEvent *event,
                                  __attribute__((unused)) gpointer uData) {
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
#if HW_SIM_PERSISTS
   gboolean self_deleting = HW_IS_SIM_PANEL(srcWidget);
   logging_llprintf(LOGLEVEL_DEBUG, "%s: source is self - %s", __func__, self_deleting ? "true":"false");
   if (self_deleting){
      gtk_widget_hide(srcWidget);
      return TRUE;
   }
#endif

   return FALSE;
}
