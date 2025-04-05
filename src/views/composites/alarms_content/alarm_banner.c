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
   GtkBox   *banner_hbox;
   GtkLabel *lbl_message;
   GtkLabel *lbl_bullet;
   GtkImage *banner_icon;

   ALARM_MODEL_LEVEL alarm_level;
};

G_DEFINE_TYPE(AlarmBanner, alarm_banner, GTK_TYPE_BOX)

static void alarm_banner_finalize(GObject *self);

static void alarm_banner_class_init(AlarmBannerClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = alarm_banner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (klass), "/resource_path/resources/alarm_banner.ui");
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, banner_hbox);
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, lbl_message);
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, lbl_bullet);
   gtk_widget_class_bind_template_child(widget_class, AlarmBanner, banner_icon);

}

static void alarm_banner_init(AlarmBanner *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
   self->alarm_level = AM_NO_ALARM;
}

AlarmBanner *alarm_banner_new()
{
   AlarmBanner *myself;
   myself = g_object_new(ALARM_TYPE_BANNER, NULL);

   GtkStyleContext *dlgContext;
   dlgContext = gtk_widget_get_style_context(GTK_WIDGET(myself));
   gtk_style_context_add_class(dlgContext, "alarm-none");

   return myself;
}

static void alarm_banner_finalize(GObject *obj)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(obj != NULL);
   g_return_if_fail(ALARM_IS_BANNER(obj));

   G_OBJECT_CLASS(alarm_banner_parent_class)->finalize(obj);
}

void alarm_banner_set_alarm_level(AlarmBanner *self, ALARM_MODEL_LEVEL level)
{
#define SOME_DEFAULT_MESSAGE "<b>Markup Allowed Message</b>"
#define ALARM_LOW_LABEL_MARKUP "<b>LOW</b>"
#define ALARM_MID_LABEL_MARKUP "<b>MID</b>"
#define ALARM_HIGH_LABEL_MARKUP "<b>HIGH</b>"

   GtkStyleContext *dlgContext;
   dlgContext = gtk_widget_get_style_context(GTK_WIDGET(self));

   if (level == AM_NO_ALARM)
   {
      gtk_label_set_markup(GTK_LABEL(self->lbl_message), SOME_DEFAULT_MESSAGE);
      gtk_style_context_add_class(dlgContext, "alarm-none");
      gtk_style_context_remove_class(dlgContext, "alarm-high");
      gtk_style_context_remove_class(dlgContext, "alarm-mid");
      gtk_style_context_remove_class(dlgContext, "alarm-low");
   }
   else if (level < AM_BASIC_MID_ALARM)
   { // AM_XXXX_LOW_ALARM == [1:AM_BASIC_MID_ALARM)
      gtk_label_set_markup(GTK_LABEL(self->lbl_message), ALARM_LOW_LABEL_MARKUP);
      gtk_style_context_remove_class(dlgContext, "alarm-none");
      gtk_style_context_remove_class(dlgContext, "alarm-high");
      gtk_style_context_remove_class(dlgContext, "alarm-mid");
      gtk_style_context_add_class(dlgContext, "alarm-low");
   }
   else if (level < AM_BASIC_HIGH_ALARM)
   { // AM_XXXX_MID_ALARM == [AM_BASIC_MID_ALARM:AM_BASIC_HIGH_ALARM)
      gtk_label_set_markup(GTK_LABEL(self->lbl_message), ALARM_MID_LABEL_MARKUP);
      gtk_style_context_remove_class(dlgContext, "alarm-none");
      gtk_style_context_remove_class(dlgContext, "alarm-high");
      gtk_style_context_add_class(dlgContext, "alarm-mid");
      gtk_style_context_remove_class(dlgContext, "alarm-low");
   }
   else
   { // AM_XXXX_HIGH_ALARM == [AM_BASIC_HIGH_ALARM:INT_MAX?)
      gtk_label_set_markup(GTK_LABEL(self->lbl_message), ALARM_HIGH_LABEL_MARKUP);
      gtk_style_context_remove_class(dlgContext, "alarm-none");
      gtk_style_context_add_class(dlgContext, "alarm-high");
      gtk_style_context_remove_class(dlgContext, "alarm-mid");
      gtk_style_context_remove_class(dlgContext, "alarm-low");
   }

}
