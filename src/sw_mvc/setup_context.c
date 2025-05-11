/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "setup_context.h"
#include "context_mediator.h"
#include "utils/logging.h"

struct _SetupContext
{
   GtkBox super;
   GtkBox *root_content;
   GtkButton *btn_setup_complete;

   ContextMediatorUpdateFn_T setup_context_update_model_view;

   RunModel *model;
};

void setup_context_update_model_view(ContextMediator *iface_self, RunModel *model);

static void setup_context_iface_init(ContextMediatorInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(iface != NULL);
   iface->update_model_view = setup_context_update_model_view;
}

G_DEFINE_TYPE_WITH_CODE(SetupContext, setup_context, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE(CONTEXT_TYPE_MEDIATOR, setup_context_iface_init))

static void setup_context_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SETUP_IS_CONTEXT(g_object));

   G_OBJECT_CLASS(setup_context_parent_class)->finalize(g_object);
}

void on_btn_setup_complete_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   SetupContext *self = SETUP_CONTEXT(user_data);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   setup_context_update_model_view(CONTEXT_MEDIATOR(self), self->model);
}

static void setup_context_class_init(SetupContextClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = setup_context_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/setup_context");
   gtk_widget_class_bind_template_child(widget_class, SetupContext, root_content);
   gtk_widget_class_bind_template_child(widget_class, SetupContext, btn_setup_complete);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_setup_complete_clicked", (GCallback)on_btn_setup_complete_clicked);
}

static void setup_context_init(SetupContext *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_type_ensure(CONTEXT_TYPE_MEDIATOR);

   gtk_widget_init_template(GTK_WIDGET(self));
}

SetupContext *setup_context_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupContext *self;
   self = g_object_new(SETUP_TYPE_CONTEXT, NULL);

   self->model = model;

   return self;
}

void setup_context_update_model_view(ContextMediator *iface_self, RunModel *model)
{
   SetupContext *self = SETUP_CONTEXT(iface_self);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
