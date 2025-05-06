/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "hw_sim_panel.h"
#include "utils/logging.h"

struct _HwSimPanel
{
   GtkWindow      super;

   GtkButton      *hw_sim_panel_close;
   GtkButton      *btn_pump_enable_response;
   GtkButton      *btn_heat_enable_response;

   GtkCheckButton *ckbtn_heat_enable_failure;
   GtkCheckButton *ckbtn_pump_enable_failure;
};

G_DEFINE_TYPE(HwSimPanel, hw_sim_panel, GTK_TYPE_WINDOW)

static void hw_sim_panel_finalize(GObject *g_object)
{
//   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_print("%s\n", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(HW_IS_SIM_PANEL(g_object));
//   HwSimPanel *self = HW_SIM_PANEL(g_object);

   G_OBJECT_CLASS(hw_sim_panel_parent_class)->finalize(g_object);
}

gboolean on_hw_sim_panel_delete_event(GtkWidget *srcWidget, GdkEvent *event, gpointer uData);
static void on_hw_sim_panel_destroy(GtkWidget *self);
static void on_hw_sim_panel_close_clicked(GtkWidget *button, gpointer *user_data);
static void on_btn_pump_enable_response_clicked(GtkButton *button, gpointer *user_data);
static void on_btn_heat_enable_response_clicked(GtkButton *button, gpointer *user_data);

static void on_ckbtn_pump_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);
static void on_ckbtn_heat_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data);

static void hw_sim_panel_class_init(HwSimPanelClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = hw_sim_panel_finalize;
//   widget_class->delete_event = (void *)on_hw_sim_panel_delete_event;
//   widget_class->destroy =(void *)on_hw_sim_panel_destroy;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/hw_sim_panel");

   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, hw_sim_panel_close);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_pump_enable_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, btn_heat_enable_response);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_heat_enable_failure);
   gtk_widget_class_bind_template_child_internal(widget_class, HwSimPanel, ckbtn_pump_enable_failure);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_delete_event", (GCallback)on_hw_sim_panel_delete_event);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_destroy", (GCallback)on_hw_sim_panel_destroy);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_hw_sim_panel_close_clicked", (GCallback)on_hw_sim_panel_close_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_pump_enable_response_clicked", (GCallback)on_btn_pump_enable_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_heat_enable_response_clicked", (GCallback)on_btn_heat_enable_response_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_pump_enable_failure_toggled", (GCallback)on_ckbtn_pump_enable_failure_toggled);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_heat_enable_failure_toggled", (GCallback)on_ckbtn_heat_enable_failure_toggled);
}

static void hw_sim_panel_init(HwSimPanel *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(HW_IS_SIM_PANEL(self));
   gtk_widget_init_template(GTK_WIDGET(self));
}

HwSimPanel *hw_sim_panel_new()
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   HwSimPanel *self;
   self = g_object_new(HW_TYPE_SIM_PANEL, NULL);

   return self;
}

/////////////////////
static void on_hw_sim_panel_close_clicked(GtkWidget *button, gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_window_close(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
}

static void on_btn_pump_enable_response_clicked(GtkButton *button, gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_btn_heat_enable_response_clicked(GtkButton *button, gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void on_ckbtn_pump_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void on_ckbtn_heat_enable_failure_toggled(GtkToggleButton *button, gpointer *user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void on_hw_sim_panel_destroy(GtkWidget *self)
{
   gtk_widget_destroyed(self, &self);
}

gboolean on_hw_sim_panel_delete_event(__attribute__((unused)) GtkWidget *srcWidget,
                                  __attribute__((unused)) GdkEvent *event,
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
