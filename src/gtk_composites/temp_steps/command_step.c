/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "command_step.h"
#include "utils/logging.h"


struct _CommandStep
{
   GtkBox      parent;
   GtkLabel    *lbl_command_bullet;
   GtkLabel    *lbl_command_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;
};

G_DEFINE_TYPE(CommandStep, command_step, GTK_TYPE_BOX)

static void command_step_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(COMMAND_IS_STEP(g_object));

   G_OBJECT_CLASS(command_step_parent_class)->finalize(g_object);
}

void on_command_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   CommandStep *sa = COMMAND_STEP(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_command_bullet, BLUE_BULLET_FORMAT_STR);
}

void on_command_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void command_step_class_init(CommandStepClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = command_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/command_step");
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_command_bullet);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_command_description);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_command_step_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_command_step_btn_cancel_skip_clicked);
}

static void command_step_init(CommandStep *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
}

CommandStep *command_step_new(const gchar *command_description, RunModel *model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   CommandStep *sa;
   sa = g_object_new(COMMAND_TYPE_STEP, NULL);

   gtk_label_set_text(sa->lbl_command_description, command_description);

   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_command_bullet, BLUE_BULLET_FORMAT_STR);

   return sa;
}
