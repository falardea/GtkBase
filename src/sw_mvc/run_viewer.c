/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "run_viewer.h"
#include "utils/logging.h"

typedef struct {
   GtkWidget   *setup_interface;
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

G_DEFINE_TYPE_WITH_PRIVATE(RunViewer, run_viewer, G_TYPE_OBJECT)

static void run_viewer_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(RUN_IS_VIEWER(g_object));

   G_OBJECT_CLASS(run_viewer_parent_class)->finalize(g_object);
}

static void run_viewer_class_init(RunViewerClass *klass)
{
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
   priv->setup_interface = NULL;
}

RunViewer *run_viewer_new(RunModel *model)
{
   g_return_val_if_fail(model != NULL, NULL);

   RunViewer *self;
   self = g_object_new(RUN_TYPE_VIEWER, NULL);
   return self;
}
