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
   GtkButton   *btn_acknowledge;

   void        (*on_user_acknowledge)(SequenceRunner *parent_sequence, gpointer user_data);
   gpointer    callback_user_data;

   SequenceRunner *parent_sequence;
};

static void step_acknowledge_execute(StepExecutable *self)
{
   StepAcknowledge *sa = STEP_ACKNOWLEDGE(self);
   logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, gtk_label_get_label(sa->lbl_step_description));
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_acknowledge), TRUE);
}

static void step_acknowledge_executable_interface_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
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

void on_btn_acknowledge_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   StepAcknowledge *sa = STEP_ACKNOWLEDGE(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_acknowledge), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   sa->on_user_acknowledge(sa->parent_sequence, sa->callback_user_data);
}

static void step_acknowledge_class_init(StepAcknowledgeClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_acknowledge_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_acknowledge.ui");
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, btn_acknowledge);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_acknowledge_clicked", (GCallback)on_btn_acknowledge_clicked);
}

static void step_acknowledge_init(StepAcknowledge *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_type_ensure(STEP_TYPE_EXECUTABLE);

   gtk_widget_init_template(GTK_WIDGET(self));
}

StepAcknowledge *step_acknowledge_new(const gchar *step_description,
                                      const gchar *btn_label_str,
                                      SequenceRunner *parent_sequence,
                                      SequenceCallback_T on_acknowledge,
                                      gpointer callback_user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   StepAcknowledge *sa;
   sa = g_object_new(STEP_TYPE_ACKNOWLEDGE, NULL);

   sa->on_user_acknowledge = on_acknowledge;
   sa->callback_user_data = callback_user_data;
   sa->parent_sequence = parent_sequence;

   gtk_label_set_text(sa->lbl_step_description, step_description);
   gtk_button_set_label(sa->btn_acknowledge, btn_label_str);

   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_acknowledge), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   return sa;
}
