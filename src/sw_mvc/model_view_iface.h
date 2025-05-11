/**
 * Created by french on 5/10/25.
 * @brief
 */
#ifndef MODEL_VIEW_IFACE_H__
#define MODEL_VIEW_IFACE_H__
#include <glib-object.h>

#include "app_model.h"

G_BEGIN_DECLS

#define MODEL_TYPE_IFACE (model_iface_get_type())

typedef struct _ModelIface ModelIface;

G_DECLARE_INTERFACE (ModelIface, model_iface, MODEL, IFACE, GObject)

struct _ModelIfaceInterface
{
   GTypeInstance super;
   void (*update_model_view)(ModelIface *self, AppModel *model);

   gpointer padding[10];

};

void model_iface_update_model_view(ModelIface *self, AppModel *model);

G_END_DECLS
#endif  // MODEL_VIEW_IFACE_H__
