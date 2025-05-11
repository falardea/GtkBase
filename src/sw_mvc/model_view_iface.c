/**
 * Created by french on 5/10/25.
 * @brief
 */
#include "model_view_iface.h"

G_DEFINE_INTERFACE(ModelIface, model_iface, G_TYPE_OBJECT)

static void model_iface_default_init(ModelIfaceInterface *iface){}

void model_iface_update_model_view(ModelIface *self, AppModel *model)
{
   ModelIfaceInterface *iface;

   g_return_if_fail(MODEL_IS_IFACE(self));
   iface = MODEL_IFACE_GET_IFACE(self);
   g_return_if_fail(iface->update_model_view != NULL);
   iface->update_model_view(self, model);
}
