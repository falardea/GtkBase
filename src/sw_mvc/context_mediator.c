/**
 * Created by french on 5/10/25.
 * @brief
 */
#include "context_mediator.h"
#include "utils/logging.h"

G_DEFINE_INTERFACE(ContextMediator, context_mediator, G_TYPE_OBJECT)

static void context_mediator_default_init(ContextMediatorInterface *iface){}

void context_mediator_update_model_view(ContextMediator *self, RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ContextMediatorInterface *iface;

   g_return_if_fail(CONTEXT_IS_MEDIATOR(self));
   iface = CONTEXT_MEDIATOR_GET_IFACE(self);
   g_return_if_fail(iface->update_model_view != NULL);
   iface->update_model_view(self, model);
}
