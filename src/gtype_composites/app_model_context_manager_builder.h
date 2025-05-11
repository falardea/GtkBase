/**
 * Created by french on 5/10/25.
 * @brief
 */
#ifndef APP_MODEL_CONTEXT_MANAGER_BUILDER_H__
#define APP_MODEL_CONTEXT_MANAGER_BUILDER_H__
#include <gtk/gtk.h>
#include "app_model.h"

G_BEGIN_DECLS

GtkBox *build_context_for_app_mode(AppModel *model);

G_END_DECLS
#endif  // APP_MODEL_CONTEXT_MANAGER_BUILDER_H__
