/**
 * Created by french on 5/10/25.
 * @brief
 */
#ifndef CONTEXT_MEDIATOR_H__
#define CONTEXT_MEDIATOR_H__
#include <glib-object.h>

#include "run_model.h"

G_BEGIN_DECLS

typedef struct _RunModel RunModel;
typedef struct _ContextMediator ContextMediator;

#define CONTEXT_TYPE_MEDIATOR (context_mediator_get_type())

G_DECLARE_INTERFACE (ContextMediator, context_mediator, CONTEXT, MEDIATOR, GObject)

typedef void (*ContextMediatorUpdateFn_T)(ContextMediator *mediator_self, RunModel *model);

struct _ContextMediatorInterface
{
   GTypeInstance super;
   ContextMediatorUpdateFn_T update_model_view;

   gpointer padding[10];

};

void context_mediator_update_model_view(ContextMediator *self, RunModel *model);

G_END_DECLS
#endif  // CONTEXT_MEDIATOR_H__
