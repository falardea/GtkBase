/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "service_context.h"
#include "context_mediator.h"
#include "utils/logging.h"

struct _ServiceContext
{
   GtkBox super;
   GtkBox *root_content;

   ContextMediatorUpdateFn_T service_context_update_model_view;

   RunModel *model;
};

void service_context_update_model_view(ContextMediator *iface_self, RunModel *model);

static void setup_context_iface_init(ContextMediatorInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(iface != NULL);
   iface->update_model_view = service_context_update_model_view;
}

G_DEFINE_TYPE_WITH_CODE(ServiceContext, service_context, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE(CONTEXT_TYPE_MEDIATOR, setup_context_iface_init))

static void service_context_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SERVICE_IS_CONTEXT(g_object));
   ServiceContext *self = SERVICE_CONTEXT(g_object);
   G_OBJECT_CLASS(service_context_parent_class)->finalize(g_object);
}

static void service_context_class_init(ServiceContextClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = service_context_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/service_context");
   gtk_widget_class_bind_template_child(widget_class, ServiceContext, root_content);

}

static void service_context_init(ServiceContext *self)
{
   g_type_ensure(CONTEXT_TYPE_MEDIATOR);

   gtk_widget_init_template(GTK_WIDGET(self));
}

ServiceContext *service_context_new(RunModel *model)
{
   ServiceContext *self;
   self = g_object_new(SERVICE_TYPE_CONTEXT, NULL);
   self->model = model;
   return self;
}

void service_context_update_model_view(ContextMediator *iface_self, RunModel *model)
{
   ServiceContext *self = SERVICE_CONTEXT(iface_self);
}
