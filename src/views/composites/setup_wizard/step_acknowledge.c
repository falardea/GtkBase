/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_acknowledge.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

struct _StepAcknowledge
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;

   void        (*on_user_acknowledge)(StepExecutable *parent_sequence, RunModel *run_model);
   gpointer    callback_user_data;

   StepExecutable *parent_sequence;
};

static void step_acknowledge_execute(StepExecutable *self,__attribute__((unused)) RunModel *run_model)
{
   StepAcknowledge *sa = STEP_ACKNOWLEDGE(self);
   logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, gtk_label_get_label(sa->lbl_step_description));
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), TRUE);
}

static void step_acknowledge_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = step_acknowledge_execute;
}

G_DEFINE_TYPE_WITH_CODE(StepAcknowledge, step_acknowledge, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_acknowledge_executable_interface_init))

static void step_acknowledge_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_ACKNOWLEDGE(g_object));

   G_OBJECT_CLASS(step_acknowledge_parent_class)->finalize(g_object);
}

void on_step_acknowledge_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   StepAcknowledge *sa = STEP_ACKNOWLEDGE(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   if (sa->on_user_acknowledge != NULL)
   {
      sa->on_user_acknowledge(STEP_EXECUTABLE(sa), sa->callback_user_data);
   }
   if (sa->parent_sequence != NULL)
   {
      step_executable_execute(sa->parent_sequence, sa->callback_user_data);
   }
}

void on_step_acknowledge_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void step_acknowledge_class_init(StepAcknowledgeClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_acknowledge_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_acknowledge.ui");
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_step_acknowledge_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_step_acknowledge_btn_cancel_skip_clicked);
}

static void step_acknowledge_init(StepAcknowledge *self)
{
   g_type_ensure(STEP_TYPE_EXECUTABLE);

   gtk_widget_init_template(GTK_WIDGET(self));
}

StepAcknowledge *step_acknowledge_new(const gchar *step_description,
                                      const gchar *btn_label_str,
                                      StepExecutable *parent_sequence,
                                      ExecutableCallback_T on_acknowledge,
                                      gpointer callback_user_data)
{
   StepAcknowledge *sa;
   sa = g_object_new(STEP_TYPE_ACKNOWLEDGE, NULL);

   sa->on_user_acknowledge = on_acknowledge;
   sa->callback_user_data = callback_user_data;
   sa->parent_sequence = parent_sequence;

   gtk_label_set_text(sa->lbl_step_description, step_description);
   gtk_button_set_label(sa->btn_start_next, btn_label_str);

   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   return sa;
}
