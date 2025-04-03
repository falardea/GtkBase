/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "alarms_content.h"
#include "alarm_banner.h"
#include "alarm_interface.h"

struct _AlarmsContent
{
   GtkBox         parent;
   AlarmBanner    *alarm_banner;
   AlarmInterface *alarm_interface;
};

G_DEFINE_TYPE(AlarmsContent, alarms_content, GTK_TYPE_BOX)

static void alarms_content_finalize(GObject *self);

static void alarms_content_class_init(AlarmsContentClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = alarms_content_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/resources/alarms_content.ui");
   gtk_widget_class_bind_template_child(widget_class, AlarmsContent, alarm_banner);
   gtk_widget_class_bind_template_child(widget_class, AlarmsContent, alarm_interface);

}

static void alarms_content_init(AlarmsContent *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

AlarmsContent *alarms_content_new()
{
   AlarmsContent *myself;
   myself = g_object_new(ALARMS_TYPE_CONTENT, NULL);

   return myself;
}

static void alarms_content_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(ALARMS_IS_CONTENT(self));

   G_OBJECT_CLASS(alarms_content_parent_class)->finalize(self);
}