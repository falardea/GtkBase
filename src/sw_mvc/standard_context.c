/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "standard_context.h"
#include "context_mediator.h"
#include "utils/logging.h"

struct _StandardContext
{
   GtkBox super;
   GtkBox *root_content;

   ContextMediatorUpdateFn_T standard_context_update_model_view;

   RunModel *model;
};

void standard_context_update_model_view(ContextMediator *iface_self, RunModel *model);

static void standard_context_iface_init(ContextMediatorInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(iface != NULL);
   iface->update_model_view = standard_context_update_model_view;
}

G_DEFINE_TYPE_WITH_CODE(StandardContext, standard_context, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE(CONTEXT_TYPE_MEDIATOR, standard_context_iface_init))

static void standard_context_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STANDARD_IS_CONTEXT(g_object));
   StandardContext *self = STANDARD_CONTEXT(g_object);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS(standard_context_parent_class)->finalize(g_object);
}

static void standard_context_class_init(StandardContextClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = standard_context_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/standard_context");
   gtk_widget_class_bind_template_child(widget_class, StandardContext, root_content);
}

static void standard_context_init(StandardContext *self)
{
   g_type_ensure(CONTEXT_TYPE_MEDIATOR);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   gtk_widget_init_template(GTK_WIDGET(self));
}

StandardContext *standard_context_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   StandardContext *self;
   self = g_object_new(STANDARD_TYPE_CONTEXT, NULL);
   self->model = model;

   return self;
}

void standard_context_update_model_view(ContextMediator *iface_self, RunModel *model)
{
   StandardContext *self = STANDARD_CONTEXT(iface_self);
}
