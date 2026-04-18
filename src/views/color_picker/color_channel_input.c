/*
 * Created by french on 4/11/26.
*/
#include <math.h>
#include <stdbool.h>
#include "color_channel_input.h"

#define CHANNEL_VALUE_CHANGED_PROPERTY_STR "channel-value-changed"

struct _ColorChannelInput
{
   GtkBox parent;

   GtkLabel *display_label;
   GtkLabel *hex_label;

   GtkScale *slider_scale;
   GtkAdjustment *slider_adjustment;

   GtkEntry *value_entry;

   char *channel_name;
   float current_value;
};

G_DEFINE_TYPE(ColorChannelInput, color_channel_input, GTK_TYPE_BOX)

static void color_channel_input_finalize(GObject *obj_self);
static void on_value_entry_changed(__attribute__((unused))GtkEditable *entry, gpointer user_data);
static void on_slider_scale_value_changed(GtkScrollbar *bar, gpointer user_data);

static bool checkedStrtof(const char *token, float *res)
{
   // Make sure the input is not a null pointer or an empty string
   if ((token == NULL) || (token[0] == '\0'))
   {
      g_print("Empty string when converting to float\n");
      return false;
   }

   char *endptr = NULL;
   errno = 0;

   // Convert to a float
   *res = strtof(token, &endptr);

   // Check that conversion succeeded
   if (errno != 0)
   {
      perror("strtof");
      return false;
   }

   // Check if there is anything after the token - this would be an error for us
   if (*endptr != '\0')
   {
      g_print("Extra characters remained after converting to float\n");
      return false;
   }

   return true;
}

static void color_channel_input_update_displayed_value(ColorChannelInput *self)
{
   g_return_if_fail(self != NULL);

   char hexStr[32];
   snprintf(hexStr, sizeof(hexStr), "#%02X", (int)(self->current_value * 255));
   gtk_label_set_text(self->hex_label, hexStr);

   char valueStr[32];
   snprintf(valueStr, sizeof(valueStr), "%.*f", 0, (float)(self->current_value  * 255));

   g_signal_handlers_block_by_func(G_OBJECT(self->value_entry), on_value_entry_changed, self);
   gtk_entry_set_text(self->value_entry, valueStr);
   g_signal_handlers_unblock_by_func(G_OBJECT(self->value_entry), on_value_entry_changed, self);

   g_signal_handlers_block_by_func(G_OBJECT(self->slider_scale), on_slider_scale_value_changed, self);
   gtk_range_set_value(GTK_RANGE(self->slider_scale), self->current_value);
   g_signal_handlers_unblock_by_func(G_OBJECT(self->slider_scale), on_slider_scale_value_changed, self);
}

static void on_slider_scale_value_changed(GtkScrollbar *bar, gpointer user_data)
{
   ColorChannelInput *self = (ColorChannelInput *)user_data;
   gdouble slider_value = gtk_range_get_value(GTK_RANGE(self->slider_scale));

   color_channel_input_set_value(self, (float)slider_value);
}

static void on_value_entry_changed(__attribute__((unused))GtkEditable *entry, gpointer user_data)
{
   ColorChannelInput *self = (ColorChannelInput *)user_data;

   const char *entry_str = gtk_entry_get_text(GTK_ENTRY(self->value_entry));
   if ('\0' != entry_str[0])
   {
      float entry_value;
      bool valid_input = checkedStrtof(entry_str, &entry_value);

      if (valid_input)
      {
         color_channel_input_set_value(self, entry_value);
      }
   }
}

enum
{
   PROP_0 = 0, // reserved for the base GObject
   CCI_VALUE_CHANGED,
   CCI_N_PROPERTIES
};

static void color_channel_input_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   ColorChannelInput *self = COLOR_CHANNEL_INPUT(object);
   if (prop_id == CCI_VALUE_CHANGED){
      color_channel_input_set_value(self, g_value_get_float( value ) );
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void color_channel_input_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   ColorChannelInput *self = COLOR_CHANNEL_INPUT(object);
   if (prop_id == CCI_VALUE_CHANGED){
      g_value_set_float(value, color_channel_input_get_value(self));
   }
   else
   {
      G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *channel_properties[CCI_N_PROPERTIES] = {NULL, };

static void color_channel_input_class_init(ColorChannelInputClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS( klass );
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = color_channel_input_finalize;

   gobject_class->get_property = color_channel_input_get_property;
   gobject_class->set_property = color_channel_input_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/color_channel_input");
   gtk_widget_class_bind_template_child(widget_class, ColorChannelInput, slider_adjustment);
   gtk_widget_class_bind_template_child(widget_class, ColorChannelInput, display_label);
   gtk_widget_class_bind_template_child(widget_class, ColorChannelInput, slider_scale);
   gtk_widget_class_bind_template_child(widget_class, ColorChannelInput, hex_label);
   gtk_widget_class_bind_template_child(widget_class, ColorChannelInput, value_entry);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_value_entry_changed", (GCallback)on_value_entry_changed);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_slider_scale_value_changed", (GCallback)on_slider_scale_value_changed);

   channel_properties[CCI_VALUE_CHANGED] = g_param_spec_float(CHANNEL_VALUE_CHANGED_PROPERTY_STR, NULL, NULL,
                                                              0.0f, 255.0f, 0.0f, G_PARAM_READWRITE  | G_PARAM_STATIC_NAME);

   g_object_class_install_properties( gobject_class, CCI_N_PROPERTIES, channel_properties);

}

void color_channel_input_set_value(ColorChannelInput *self, float value)
{
   if ((value != NAN) && (fabs((double)(value - self->current_value)) > 0.01))
   {
      self->current_value = value;

      color_channel_input_update_displayed_value(self);

      g_object_notify_by_pspec(G_OBJECT(self), channel_properties[CCI_VALUE_CHANGED]);
   }
}

float color_channel_input_get_value(ColorChannelInput *self)
{
   return self->current_value;
}

static void color_channel_input_init(ColorChannelInput *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

ColorChannelInput *color_channel_input_new(const char *name)
{
   ColorChannelInput *self;
   self = g_object_new(COLOR_CHANNEL_TYPE_INPUT, NULL);

   gtk_label_set_text(self->display_label, name);

   return self;
}

static void color_channel_input_finalize(GObject *obj_self)
{
   g_return_if_fail(obj_self != NULL);
   g_return_if_fail(COLOR_CHANNEL_IS_INPUT(obj_self));
   ColorChannelInput *self = COLOR_CHANNEL_INPUT(obj_self);
   if (self->channel_name != NULL)
   {
      g_free(self->channel_name);
   }
   G_OBJECT_CLASS(color_channel_input_parent_class)->finalize(obj_self);
}

