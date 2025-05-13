/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "branch_step.h"
#include "template_common.h"
#include "utils/logging.h"

struct _BranchStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_cancel_skip;
};

G_DEFINE_TYPE(BranchStep, branch_step, GTK_TYPE_BOX)

static void branch_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(BRANCH_IS_STEP(g_object));
   BranchStep *self = BRANCH_STEP(g_object);
   G_OBJECT_CLASS(branch_step_parent_class)->finalize(g_object);
}

void on_branch_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}


static void branch_step_class_init(BranchStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = branch_step_finalize;


   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/branch_step");
   gtk_widget_class_bind_template_child(widget_class, BranchStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, BranchStep, btn_cancel_skip);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_branch_step_btn_cancel_skip_clicked);

}

static void branch_step_init(BranchStep *self)
{
   g_return_if_fail(BRANCH_IS_STEP(self));
   gtk_widget_init_template(GTK_WIDGET(self));
}

BranchStep *branch_step_new(const gchar *branch_description)
{
   BranchStep *self;
   self = g_object_new(BRANCH_TYPE_STEP, NULL);

   if (branch_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, branch_description);
   }
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_cancel_skip), FALSE);

   return self;
}
