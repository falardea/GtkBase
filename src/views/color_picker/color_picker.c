/*
 * Created by french on 4/17/26.
*/
#include <math.h>
#include "color_picker.h"
#include "color_channel_input.h"

#define RED_COLOR_CHANGED_PROPERTY_STR "red-changed"
#define GREEN_COLOR_CHANGED_PROPERTY_STR "green-changed"
#define BLUE_COLOR_CHANGED_PROPERTY_STR "blue-changed"

typedef struct {
   ColorChannelInput *red_channel;
   ColorChannelInput *green_channel;
   ColorChannelInput *blue_channel;
} ColorPickerPrivate;

struct _ColorPicker
{
   GtkBox parent;

   GtkDrawingArea *color_output;
   GtkBox         *slider_box;

   float red;
   float green;
   float blue;
};

G_DEFINE_TYPE_WITH_PRIVATE(ColorPicker, color_picker, GTK_TYPE_BOX)

gboolean color_picker_color_output_draw_func(GtkWidget* widget, cairo_t *cr, __attribute__((unused))gpointer user_data);
void color_picker_set_red(ColorPicker *self, float red);
void color_picker_set_green(ColorPicker *self, float green);
void color_picker_set_blue(ColorPicker *self, float blue);

static void color_picker_finalize(GObject *g_self)
{
   ColorPicker *self = COLOR_PICKER(g_self);
   ColorPickerPrivate *priv = color_picker_get_instance_private(self);

   if(priv->red_channel)
   {
      g_object_unref(G_OBJECT(priv->red_channel));
   }
   if(priv->green_channel)
   {
      g_object_unref(G_OBJECT(priv->green_channel));
   }
   if(priv->blue_channel){
      g_object_unref(G_OBJECT(priv->blue_channel));
   }
   G_OBJECT_CLASS(color_picker_parent_class)->finalize(g_self);
}

enum
{
   PROP_0 = 0, // reserved for the base GObject
   CP_RED_COLOR_CHANGED,
   CP_GREEN_COLOR_CHANGED,
   CP_BLUE_COLOR_CHANGED,
   CP_N_PROPERTIES
};

static void color_picker_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   ColorPicker *self = COLOR_PICKER(object);
   switch(prop_id)
   {
      case CP_RED_COLOR_CHANGED:
         color_picker_set_red(self, g_value_get_float( value ) );
         break;
      case CP_GREEN_COLOR_CHANGED:
         color_picker_set_green(self, g_value_get_float( value ) );
         break;
      case CP_BLUE_COLOR_CHANGED:
         color_picker_set_blue(self, g_value_get_float( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
         break;
   }
}

static GParamSpec *color_picker_properties[CP_N_PROPERTIES] = {NULL, };

static void color_picker_class_init(ColorPickerClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *)klass;

   gobject_class->finalize = color_picker_finalize;
   gobject_class->set_property = color_picker_set_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/color_picker");
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, color_output);
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, slider_box);

   gtk_widget_class_bind_template_callback_full(widget_class, "draw", (GCallback)color_picker_color_output_draw_func);

   color_picker_properties[CP_RED_COLOR_CHANGED] = g_param_spec_float(RED_COLOR_CHANGED_PROPERTY_STR, NULL, NULL,
                                                                      0.0f, 255.0f, 0.0f, G_PARAM_WRITABLE  | G_PARAM_STATIC_NAME);
   color_picker_properties[CP_GREEN_COLOR_CHANGED] = g_param_spec_float(GREEN_COLOR_CHANGED_PROPERTY_STR, NULL, NULL,
                                                                        0.0f, 255.0f, 0.0f, G_PARAM_WRITABLE  | G_PARAM_STATIC_NAME);
   color_picker_properties[CP_BLUE_COLOR_CHANGED] = g_param_spec_float(BLUE_COLOR_CHANGED_PROPERTY_STR, NULL, NULL,
                                                                       0.0f, 255.0f, 0.0f, G_PARAM_WRITABLE  | G_PARAM_STATIC_NAME);

   g_object_class_install_properties( gobject_class, CP_N_PROPERTIES, color_picker_properties);

}

static void color_picker_init(ColorPicker *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

ColorPicker *color_picker_new()
{
   ColorPicker *self;
   self = g_object_new(COLOR_TYPE_PICKER, NULL);

   ColorPickerPrivate *priv = color_picker_get_instance_private(self);

   self->red = 0;
   self->green = 0;
   self->blue = 0;

   priv->red_channel = color_channel_input_new("Red");
   priv->green_channel = color_channel_input_new("Green");
   priv->blue_channel = color_channel_input_new("Blue");

   gtk_box_pack_start(self->slider_box, GTK_WIDGET(priv->red_channel), TRUE, TRUE, 0);
   gtk_box_pack_start(self->slider_box, GTK_WIDGET(priv->green_channel), TRUE, TRUE, 0);
   gtk_box_pack_start(self->slider_box, GTK_WIDGET(priv->blue_channel), TRUE, TRUE, 0);

   g_object_bind_property(priv->red_channel, CHANNEL_VALUE_CHANGED_PROPERTY_STR,
                          self, RED_COLOR_CHANGED_PROPERTY_STR, G_BINDING_DEFAULT);
   g_object_bind_property(priv->green_channel, CHANNEL_VALUE_CHANGED_PROPERTY_STR,
                          self, GREEN_COLOR_CHANGED_PROPERTY_STR, G_BINDING_DEFAULT);
   g_object_bind_property(priv->blue_channel, CHANNEL_VALUE_CHANGED_PROPERTY_STR,
                          self, BLUE_COLOR_CHANGED_PROPERTY_STR, G_BINDING_DEFAULT);

   return self;
}

void color_picker_set_red(ColorPicker *self, float red)
{
   if (fabsf(self->red - red) > 0.01)
   {
      self->red = red;
      gtk_widget_queue_draw(GTK_WIDGET(self->color_output));
   }
}
void color_picker_set_green(ColorPicker *self, float green)
{
   if (fabsf(self->green - green) > 0.01)
   {
      self->green = green;
      gtk_widget_queue_draw(GTK_WIDGET(self->color_output));
   }
}
void color_picker_set_blue(ColorPicker *self, float blue)
{
   if (fabsf(self->blue - blue) > 0.01)
   {
      self->blue = blue;
      gtk_widget_queue_draw(GTK_WIDGET(self->color_output));
   }
}


gboolean color_picker_color_output_draw_func(GtkWidget* widget, cairo_t *cr, __attribute__((unused))gpointer user_data)
{
   guint width, height;
   GtkStyleContext *context;

   ColorPicker *self = COLOR_PICKER(user_data);
   ColorPickerPrivate *priv = color_picker_get_instance_private(self);

   GdkRGBA curr_color;
   curr_color.red = self->red;
   curr_color.green = self->green;
   curr_color.blue = self->blue;
   curr_color.alpha = 1.0;

   context = gtk_widget_get_style_context (widget);
   width = gtk_widget_get_allocated_width (widget);
   height = gtk_widget_get_allocated_height (widget);
   gtk_render_background (context, cr, 0, 0, width, height);
   cairo_rectangle(cr, 0, 0, width, height);

   gdk_cairo_set_source_rgba (cr, &curr_color);
   cairo_fill (cr);
   return FALSE;
}
