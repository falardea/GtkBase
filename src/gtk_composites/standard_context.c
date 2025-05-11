/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "standard_context.h"

struct _StandardContext
{
   GtkBox                       super;
};

G_DEFINE_TYPE(StandardContext, standard_context, GTK_TYPE_BOX)

static void standard_context_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STANDARD_IS_CONTEXT(g_object));
   StandardContext *self = STANDARD_CONTEXT(g_object);
   G_OBJECT_CLASS(standard_context_parent_class)->finalize(g_object);
}

static void standard_context_class_init(StandardContextClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = standard_context_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/standard_context");

}

static void standard_context_init(StandardContext *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

StandardContext *standard_context_new()
{
   StandardContext *self;
   self = g_object_new(STANDARD_TYPE_CONTEXT, NULL);
   return self;
}
