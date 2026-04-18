/*
 * Created by french on 4/11/26.
*/
#ifndef GTKSANDBOX_COLOR_CHANNEL_INPUT_H
#define GTKSANDBOX_COLOR_CHANNEL_INPUT_H
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define COLOR_CHANNEL_TYPE_INPUT (color_channel_input_get_type())

G_DECLARE_FINAL_TYPE(ColorChannelInput, color_channel_input, COLOR_CHANNEL, INPUT, GtkBox)

ColorChannelInput *color_channel_input_new(const char *name);

void color_channel_input_set_value(ColorChannelInput *self, float value);
float color_channel_input_get_value(ColorChannelInput *self);

#endif  /* GTKSANDBOX_COLOR_CHANNEL_INPUT_H */
