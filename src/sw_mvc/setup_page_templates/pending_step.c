/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "pending_step.h"
#include "template_common.h"
#include "utils/logging.h"

struct _PendingStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_cancel_skip;
};

G_DEFINE_TYPE(PendingStep, pending_step, GTK_TYPE_BOX)

static void pending_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PENDING_IS_STEP(g_object));
   PendingStep *self = PENDING_STEP(g_object);
   G_OBJECT_CLASS(pending_step_parent_class)->finalize(g_object);
}

void on_pending_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}


static GParamSpec *pending_step_properties[N_PENDING_STEP_PROPERTIES] = {NULL, };

static void pending_step_class_init(PendingStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = pending_step_finalize;


   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/pending_step");
   gtk_widget_class_bind_template_child(widget_class, PendingStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, PendingStep, btn_cancel_skip);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_pending_step_btn_cancel_skip_clicked);

}

static void pending_step_init(PendingStep *self)
{
   g_return_if_fail(PENDING_IS_STEP(self));
   gtk_widget_init_template(GTK_WIDGET(self));
}

PendingStep *pending_step_new(const gchar *pending_description)
{
   PendingStep *self;
   self = g_object_new(PENDING_TYPE_STEP, NULL);

   if (pending_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, pending_description);
   }
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_cancel_skip), FALSE);

   return self;
}
