/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "prompt_step.h"
#include "template_common.h"
#include "utils/logging.h"

struct _PromptStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;
};

G_DEFINE_TYPE(PromptStep, prompt_step, GTK_TYPE_BOX)

static void prompt_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PROMPT_IS_STEP(g_object));
   G_OBJECT_CLASS(prompt_step_parent_class)->finalize(g_object);
}

void on_prompt_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   PromptStep *self = PROMPT_STEP(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
}

void on_prompt_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void prompt_step_class_init(PromptStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = prompt_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/prompt_step");
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, PromptStep, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_prompt_step_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_prompt_step_btn_cancel_skip_clicked);
}

static void prompt_step_init(PromptStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

PromptStep *prompt_step_new(const gchar *prompt_description)
{
   PromptStep *self;
   self = g_object_new(PROMPT_TYPE_STEP, NULL);

   gtk_label_set_text(self->lbl_step_description, prompt_description);

   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   return self;
}
