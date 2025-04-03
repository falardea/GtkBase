/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "alarm_banner.h"
#include "app_globals.h"
#include "utils/logging.h"

struct _AlarmBanner
{
   GtkBox   parent;
   GtkBox   *banner_box;
   GtkLabel *banner_message;
   GtkImage *banner_icon;

   guint    id;
};

G_DEFINE_TYPE(AlarmBanner, alarm_banner, GTK_TYPE_BOX)

static void alarm_banner_finalize(GObject *self);

static void alarm_banner_class_init(AlarmBannerClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = alarm_banner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/resource_path/resources/alarm_banner.glade");
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, banner_box);
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, banner_message);
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, banner_icon);

}

static void alarm_banner_init(AlarmBanner *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

AlarmBanner *alarm_banner_new(guint id)
{
   AlarmBanner *myself;
   myself = g_object_new(ALARM_TYPE_BANNER, NULL);

   myself->id = id;
   return myself;
}

static void alarm_banner_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(ALARM_IS_BANNER(self));
   G_OBJECT_CLASS(alarm_banner_parent_class)->finalize(self);
}
