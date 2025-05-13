#include "setup_view.h"
#include "utils/logging.h"

typedef struct
{
   gboolean keep_alive;

}SetupViewerPrivate;

struct _SetupViewer
{
   GtkBox   super;
   GtkStack *page_stack;

   RunModel *model;
};

G_DEFINE_TYPE_WITH_PRIVATE(SetupViewer, setup_viewer, GTK_TYPE_BOX)

static gchar *step_id_to_page_string(RUN_SETUP_STEPS step_id);

static void setup_viewer_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS(setup_viewer_parent_class)->finalize(g_object);
}

static void setup_viewer_class_init(SetupViewerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = setup_viewer_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/setup_viewer");
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, page_stack);
}

static void setup_viewer_init(SetupViewer *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

SetupViewer *setup_viewer_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupViewer *self;
   self = g_object_new(SETUP_TYPE_VIEWER, NULL);
   self->model = model;

   return self;
}

void setup_viewer_add_page_by_state_id(SetupViewer *self, GtkWidget *page, RUN_SETUP_STEPS step_id)
{
   gtk_stack_add_named(self->page_stack, page, g_strdup_printf("%d", step_id));
}

void setup_viewer_set_page_view(SetupViewer *self, RUN_SETUP_STEPS step_id)
{
   gtk_stack_set_visible_child_name(self->page_stack, g_strdup_printf("%d", step_id));
}