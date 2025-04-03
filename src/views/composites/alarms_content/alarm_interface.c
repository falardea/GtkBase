/**
 * Created by french on 3/23/25.
 * @brief
 */
#include "alarm_interface.h"
#include "app_globals.h"
#include "utils/logging.h"

struct _AlarmInterface
{
   GtkBox      parent;
   GtkButton   *btn_set_high_alarm;
   GtkButton   *btn_set_mid_alarm;
   GtkButton   *btn_set_low_alarm;
   GtkButton   *btn_set_no_alarm;
};

G_DEFINE_TYPE(AlarmInterface, alarm_interface, GTK_TYPE_BOX)

static void alarm_interface_finalize(GObject *self);

void on_btn_set_high_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_set_mid_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_set_low_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_set_no_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void alarm_interface_class_init(AlarmInterfaceClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = alarm_interface_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/resource_path/resources/alarm_interface.glade");
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_high_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_mid_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_low_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_no_alarm);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_high_alarm_clicked", (GCallback)on_btn_set_high_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_mid_alarm_clicked", (GCallback)on_btn_set_mid_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_low_alarm_clicked", (GCallback)on_btn_set_low_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_no_alarm_clicked", (GCallback)on_btn_set_no_alarm_clicked);

}

static void alarm_interface_init(AlarmInterface *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

AlarmInterface *alarm_interface_new(void)
{
   AlarmInterface *myself;
   myself = g_object_new(ALARM_TYPE_INTERFACE, NULL);
   return myself;
}

static void alarm_interface_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(ALARM_INTERFACE(self));
   G_OBJECT_CLASS(alarm_interface_parent_class)->finalize(self);
}
