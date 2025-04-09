/**
 * Created by french on 3/23/25.
 * @brief
 */
#include "alarm_interface.h"
#include "app_globals.h"
#include "alarm_model.h"
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

static guint alarm_interface_signal = 0;

static void alarm_interface_finalize(GObject *g_object);

void (* alarm_interface_signal_cb) (AlarmInterface *self, ALARM_MODEL_LEVEL *level);

void on_btn_set_high_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_signal_emit(G_OBJECT(self), alarm_interface_signal, 0, AM_BASIC_HIGH_ALARM);
}
void on_btn_set_mid_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_signal_emit(G_OBJECT(self), alarm_interface_signal, 0, AM_BASIC_MID_ALARM);
}
void on_btn_set_low_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_signal_emit(G_OBJECT(self), alarm_interface_signal, 0, AM_BASIC_LOW_ALARM);
}
void on_btn_set_no_alarm_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AlarmInterface *self = ALARM_INTERFACE(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_signal_emit(G_OBJECT(self), alarm_interface_signal, 0, AM_NO_ALARM);
}

static void alarm_interface_class_init(AlarmInterfaceClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = alarm_interface_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/resource_path/alarm_interface.ui");
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_high_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_mid_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_low_alarm);
   gtk_widget_class_bind_template_child(widget_class, AlarmInterface, btn_set_no_alarm);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_high_alarm_clicked", (GCallback)on_btn_set_high_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_mid_alarm_clicked", (GCallback)on_btn_set_mid_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_low_alarm_clicked", (GCallback)on_btn_set_low_alarm_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_set_no_alarm_clicked", (GCallback)on_btn_set_no_alarm_clicked);

   alarm_interface_signal = g_signal_new_class_handler("alarm-changed",
                                                       G_TYPE_FROM_CLASS(klass),
                                                       G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                                       (GCallback)alarm_interface_signal_cb,
                                                       NULL,
                                                       NULL,
                                                       g_cclosure_marshal_VOID__INT,
                                                       G_TYPE_NONE, 1, G_TYPE_INT);

}

static void alarm_interface_init(AlarmInterface *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
}

AlarmInterface *alarm_interface_new(void)
{
   AlarmInterface *myself;
   myself = g_object_new(ALARM_TYPE_INTERFACE, NULL);
   return myself;
}

static void alarm_interface_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(ALARM_INTERFACE(g_object));


   G_OBJECT_CLASS(alarm_interface_parent_class)->finalize(g_object);
}
