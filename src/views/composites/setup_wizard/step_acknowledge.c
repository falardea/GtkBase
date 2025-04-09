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
   GtkImage    *img_step_bullet;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_acknowledge;
   GtkButton   *btn_skip;

   void        (*on_user_acknowledge)(SequenceRunner *parent_sequence, gpointer user_data);
   gpointer    callback_user_data;

   SequenceRunner *parent_sequence;
};

void on_btn_acknowledge_clicked(GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_skip_clicked(GtkButton *button, gpointer user_data)
{
  logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
static void step_acknowledge_iface_mapped_execute()
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void step_acknowledge_interface_init(StepExecutableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(StepAcknowledge, step_acknowledge, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE, step_acknowledge_interface_init))

static void step_acknowledge_interface_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s\n", __func__);
   iface->execute = step_acknowledge_iface_mapped_execute;
}

static void step_acknowledge_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SEQUENCE_IS_RUNNER(g_object));

   G_OBJECT_CLASS(step_acknowledge_parent_class)->finalize(g_object);
}

static void step_acknowledge_class_init(StepAcknowledgeClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_acknowledge_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_acknowledge.ui");
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, img_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, btn_acknowledge);
   gtk_widget_class_bind_template_child(widget_class, StepAcknowledge, btn_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_acknowledge_clicked", (GCallback)on_btn_acknowledge_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_skip_clicked", (GCallback)on_btn_skip_clicked);
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

   return sa;
}