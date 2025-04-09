/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "alarms_content.h"
#include "alarm_banner.h"
#include "alarm_interface.h"
#include "alarm_model.h"
#include "utils/logging.h"


// Private instance date
typedef struct
{
   AlarmInterface *alarm_interface;
   AlarmBanner    *alarm_banner;
   AlarmModel     *alarm_model;
} AlarmsContentPrivate;

// Public?
struct _AlarmsContent
{
   GtkBox   parent;
   GtkBox   *content_box;
};

G_DEFINE_TYPE_WITH_PRIVATE(AlarmsContent, alarms_content, GTK_TYPE_BOX)

void alarms_content_handle_alarm_signal(AlarmInterface *self, ALARM_MODEL_LEVEL level, gpointer user_data);

static void alarms_content_finalize(GObject *g_object);

static void alarms_content_class_init(AlarmsContentClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = alarms_content_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/alarms_content.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, AlarmsContent, content_box);

}

static void alarms_content_init(AlarmsContent *self)
{
   AlarmsContentPrivate *ap = alarms_content_get_instance_private(self);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_type_ensure(ALARM_TYPE_BANNER);
   g_type_ensure(ALARM_TYPE_INTERFACE);

   gtk_widget_init_template(GTK_WIDGET(self));

   ap->alarm_banner = alarm_banner_new();
   ap->alarm_interface = alarm_interface_new();
   ap->alarm_model = alarm_model_new();

   gtk_box_pack_start(GTK_BOX(self->content_box), GTK_WIDGET(ap->alarm_banner), TRUE, TRUE,0);
   gtk_box_pack_end(GTK_BOX(self->content_box), GTK_WIDGET(ap->alarm_interface), TRUE, TRUE,0);

   g_object_bind_property(ap->alarm_model, "alarm-level", ap->alarm_banner, "alarm-level", G_BINDING_DEFAULT);

   g_signal_connect(G_OBJECT(ap->alarm_interface), "alarm-changed", G_CALLBACK(alarms_content_handle_alarm_signal), self);
}

AlarmsContent *alarms_content_new()
{
   AlarmsContent *myself;

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   myself = g_object_new(ALARMS_TYPE_CONTENT, NULL);

   return myself;
}

static void alarms_content_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(ALARMS_IS_CONTENT(g_object));

   // AlarmsContentPrivate *ap = alarms_content_get_instance_private(ALARMS_CONTENT(g_object));

   G_OBJECT_CLASS(alarms_content_parent_class)->finalize(g_object);
}

void alarms_content_handle_alarm_signal(__attribute__((unused))AlarmInterface *self, ALARM_MODEL_LEVEL level, gpointer user_data)
{
   AlarmsContentPrivate *ap = alarms_content_get_instance_private(ALARMS_CONTENT(user_data));

   logging_llprintf(LOGLEVEL_DEBUG, "%s: handling child composite alarm signal", __func__);

   alarm_model_set_alarm_level(ap->alarm_model, level);
}
