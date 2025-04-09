/**
 * @brief The main gtk builder module
 */
#ifndef UI_BUILDER_H__
#define UI_BUILDER_H__
#include <gtk/gtk.h>
#include "app_globals.h"
G_BEGIN_DECLS

app_widget_ref_struct *app_builder(void);

G_END_DECLS
#endif  /* UI_BUILDER_H__ */
