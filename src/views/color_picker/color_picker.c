/*
 * Created by french on 4/17/26.
*/
#include "color_picker.h"
#include "color_channel_input.h"

typedef struct {
   ColorChannelInput *red_channel;
   ColorChannelInput *green_channel;
   ColorChannelInput *blue_channel;
} ColorPickerPrivate;

struct _ColorPicker
{
   GtkBox parent;

   GtkDrawingArea *color_output;
   GtkBox         *r_slider_box;
   GtkBox         *g_slider_box;
   GtkBox         *b_slider_box;

   GdkRGBA curr_color;
};

G_DEFINE_TYPE_WITH_PRIVATE(ColorPicker, color_picker, GTK_TYPE_BOX)

gboolean color_picker_color_output_draw_func(GtkWidget* widget, cairo_t *cr, __attribute__((unused))gpointer user_data);

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

static void color_picker_class_init(ColorPickerClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *)klass;

   gobject_class->finalize = color_picker_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/color_picker");
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, color_output);
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, r_slider_box);
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, g_slider_box);
   gtk_widget_class_bind_template_child(widget_class, ColorPicker, b_slider_box);

   gtk_widget_class_bind_template_callback_full(widget_class, "draw", (GCallback)color_picker_color_output_draw_func);

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

   self->curr_color.red = 0;
   self->curr_color.green = 0;
   self->curr_color.blue = 0;
   self->curr_color.alpha = 1;

   priv->red_channel = color_channel_input_new("Red");
//   priv->red_channel = color_channel_input_new("Green");
//   priv->red_channel = color_channel_input_new("Blue");

//   gtk_box_pack_start(self->r_slider_box, GTK_WIDGET(priv->red_channel), TRUE, TRUE, 0);
   g_print("Checkpoint >>>> %s\n", __func__);
//   gtk_box_pack_start(self->g_slider_box, GTK_WIDGET(priv->green_channel), TRUE, TRUE, 0);
//   gtk_box_pack_start(self->b_slider_box, GTK_WIDGET(priv->blue_channel), TRUE, TRUE, 0);
//   gtk_widget_show(GTK_WIDGET(priv->red_channel));
//   gtk_widget_set_visible(GTK_WIDGET(priv->green_channel), TRUE);
//   gtk_widget_set_visible(GTK_WIDGET(priv->blue_channel), TRUE);

   return self;
}

gboolean color_picker_color_output_draw_func(GtkWidget* widget, cairo_t *cr, __attribute__((unused))gpointer user_data)
{
   guint width, height;
   GtkStyleContext *context;

   ColorPicker *self = COLOR_PICKER(user_data);
   ColorPickerPrivate *priv = color_picker_get_instance_private(self);

   self->curr_color.red = color_channel_input_get_value(priv->red_channel);
   self->curr_color.green = color_channel_input_get_value(priv->green_channel);
   self->curr_color.blue = color_channel_input_get_value(priv->blue_channel);

   context = gtk_widget_get_style_context (widget);
   width = gtk_widget_get_allocated_width (widget);
   height = gtk_widget_get_allocated_height (widget);
   gtk_render_background (context, cr, 0, 0, width, height);
   cairo_rectangle(cr, 0, 0, width, height);
   gdk_cairo_set_source_rgba (cr, &self->curr_color);
   cairo_fill (cr);
   return FALSE;
}