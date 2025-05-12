/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "run_viewer.h"
#include "utils/logging.h"

#include "setup_context.h"
#include "service_context.h"
#include "standard_context.h"

typedef struct {
   StandardContext   *standard_ctx;
   ServiceContext    *service_ctx;
   SetupContext      *setup_ctx;
}RunViewerPrivate;

struct _RunViewer
{
   GtkBox         super;
   GtkBox         *box_run_view_root;

   GtkBox         *sensors_viewer;
   GtkNotebook    *run_interface;

   GtkLabel       *tablbl_setup_interface;
   GtkLabel       *tablbl_running_interface;
   GtkLabel       *tablbl_misc_interfaces;

   GtkBox         *box_setup_interface;
   GtkBox         *box_running_interface;
   GtkBox         *box_misc_interface;

   RunModel       *model;
};

G_DEFINE_TYPE_WITH_PRIVATE(RunViewer, run_viewer, GTK_TYPE_BOX)

static void run_viewer_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(RUN_IS_VIEWER(g_object));

   G_OBJECT_CLASS(run_viewer_parent_class)->finalize(g_object);
}

static void run_viewer_class_init(RunViewerClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = run_viewer_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/run_viewer");
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_run_view_root);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, sensors_viewer);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, run_interface);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_setup_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_running_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, tablbl_misc_interfaces);

   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_setup_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_running_interface);
   gtk_widget_class_bind_template_child(widget_class, RunViewer, box_misc_interface);
}

static void run_viewer_init(RunViewer *self)
{
   RunViewerPrivate *priv = run_viewer_get_instance_private(self);
   gtk_widget_init_template(GTK_WIDGET(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   priv->standard_ctx = standard_context_new(self->model);

   priv->service_ctx = service_context_new(self->model);
   priv->setup_ctx = setup_context_new(self->model);

   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->setup_ctx), TRUE, TRUE, 0);

   gtk_widget_set_visible(GTK_WIDGET(priv->setup_ctx), TRUE);
   gtk_widget_set_visible(GTK_WIDGET(priv->standard_ctx), FALSE);
   gtk_widget_set_visible(GTK_WIDGET(priv->service_ctx), FALSE);

   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->standard_ctx), TRUE, TRUE, 0);
   gtk_box_pack_start(self->box_setup_interface, GTK_WIDGET(priv->service_ctx), TRUE, TRUE, 0);
}

RunViewer *run_viewer_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   RunViewer *self;
   self = g_object_new(RUN_TYPE_VIEWER, NULL);
   self->model = model;
   return self;
}
