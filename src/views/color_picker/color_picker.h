/*
 * Created by french on 4/17/26.
*/
#ifndef GTKSANDBOX_COLOR_PICKER_H
#define GTKSANDBOX_COLOR_PICKER_H
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define COLOR_TYPE_PICKER (color_picker_get_type())

G_DECLARE_FINAL_TYPE(ColorPicker, color_picker, COLOR, PICKER, GtkBox)

ColorPicker *color_picker_new(void);

G_END_DECLS
#endif  /* GTKSANDBOX_COLOR_PICKER_H */
