/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "alarms_content.h"
#include "alarm_banner.h"
#include "alarm_interface.h"
#include "utils/logging.h"

struct _AlarmsContent
{
   GtkBox         parent;
   AlarmBanner    *alarm_banner;
   AlarmInterface *alarm_interface;
};

G_DEFINE_TYPE(AlarmsContent, alarms_content, GTK_TYPE_BOX)

void alarms_content_handle_alarm_signal(AlarmInterface *self, ALARM_INTERFACE_ALARM_LEVELS level, gpointer user_data);

static void alarms_content_finalize(GObject *self);

static void alarms_content_class_init(AlarmsContentClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = alarms_content_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/resources/alarms_content.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, AlarmsContent, alarm_banner);
   gtk_widget_class_bind_template_child(widget_class, AlarmsContent, alarm_interface);

}

static void alarms_content_init(AlarmsContent *self)
{
   g_type_ensure(ALARM_TYPE_BANNER);
   g_type_ensure(ALARM_TYPE_INTERFACE);

   gtk_widget_init_template(GTK_WIDGET(self));
}

AlarmsContent *alarms_content_new()
{
   AlarmsContent *myself;
   myself = g_object_new(ALARMS_TYPE_CONTENT, NULL);

//   myself->alarm_banner = alarm_banner_new(GINT_TO_POINTER(10));
//   myself->alarm_interface = alarm_interface_new();

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_signal_connect(G_OBJECT(myself->alarm_interface), "alarm-changed", G_CALLBACK(alarms_content_handle_alarm_signal), myself);

   return myself;
}

static void alarms_content_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(ALARMS_IS_CONTENT(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

//   AlarmsContent *ac = ALARMS_CONTENT(self);

//   gtk_widget_destroy(GTK_WIDGET(ac->alarm_banner));
//   gtk_widget_destroy(GTK_WIDGET(ac->alarm_interface));

   G_OBJECT_CLASS(alarms_content_parent_class)->finalize(self);
}

void alarms_content_handle_alarm_signal(__attribute__((unused))AlarmInterface *self, ALARM_INTERFACE_ALARM_LEVELS level, gpointer user_data)
{
   AlarmsContent *ac = ALARMS_CONTENT(user_data);

   logging_llprintf(LOGLEVEL_DEBUG, "%s: handling child composite alarm signal", __func__);

   if (level == ALARM_INTERFACE_HIGH_ALARM)
   {
      alarm_banner_set_alarm_high(ac->alarm_banner);
   }
}
