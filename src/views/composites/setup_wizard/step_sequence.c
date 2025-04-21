/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_sequence.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

struct _StepSequence
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;

   void (*on_sequence_start)(StepExecutable *parent_sequence, RunModel *run_model);
   gpointer callback_user_data;

   StepExecutable *parent_sequence;
};

static void step_sequence_execute(StepExecutable *self,__attribute__((unused)) RunModel *run_model)
{
   StepSequence *sa = STEP_SEQUENCE(self);
   logging_llprintf(LOGLEVEL_DEBUG, "%s: %s", __func__, gtk_label_get_label(sa->lbl_step_description));
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), TRUE);
}

static void step_sequence_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = step_sequence_execute;
}

G_DEFINE_TYPE_WITH_CODE(StepSequence, step_sequence, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_sequence_executable_interface_init))

static void step_sequence_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_SEQUENCE(g_object));

   G_OBJECT_CLASS(step_sequence_parent_class)->finalize(g_object);
}

void step_sequence_on_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   StepSequence *sa = STEP_SEQUENCE(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   sa->on_sequence_start(STEP_EXECUTABLE(sa->parent_sequence), sa->callback_user_data);
}

void step_sequence_on_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void step_sequence_class_init(StepSequenceClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_sequence_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_sequence.ui");
   gtk_widget_class_bind_template_child(widget_class, StepSequence, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepSequence, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepSequence, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, StepSequence, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)step_sequence_on_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)step_sequence_on_btn_cancel_skip_clicked);
}

static void step_sequence_init(StepSequence *self)
{
   g_type_ensure(STEP_TYPE_EXECUTABLE);

   gtk_widget_init_template(GTK_WIDGET(self));
}

StepSequence *step_sequence_new(const gchar *step_description,
                                const gchar *btn_label_str,
                                StepExecutable *parent_sequence,
                                ExecutableCallback_T on_sequence,
                                gpointer callback_user_data)
{
   StepSequence *sa;
   sa = g_object_new(STEP_TYPE_SEQUENCE, NULL);

   sa->on_sequence_start = on_sequence;
   sa->callback_user_data = callback_user_data;
   sa->parent_sequence = parent_sequence;

   gtk_label_set_text(sa->lbl_step_description, step_description);
   gtk_button_set_label(sa->btn_start_next, btn_label_str);

   gtk_widget_set_sensitive(GTK_WIDGET(sa->btn_start_next), FALSE);
   gtk_label_set_markup(sa->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   return sa;
}
