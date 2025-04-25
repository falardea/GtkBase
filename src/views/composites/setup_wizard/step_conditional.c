#include "step_conditional.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

struct _StepConditional
{
   GtkBox   *parent;
   GtkBox   *content_box;

   StepExecutable *parent_sequence;
   StepExecutable *start_step;
   StepExecutable *cond_true_step;
   StepExecutable *cond_false_step;

   gpointer    callback_user_data;
};

static void step_conditional_executable_interface_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(iface != NULL);
   iface->execute = step_conditional_execute;
}

G_DEFINE_TYPE_WITH_CODE(StepConditional, step_conditional, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_conditional_executable_interface_init))

void step_conditional_execute(StepExecutable *self, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   StepConditional *sc = STEP_CONDITIONAL(self);
   if(sc->parent_sequence != NULL)
   {
      step_executable_execute(sc->parent_sequence, sc->callback_user_data);
   }
}

static void step_conditional_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_CONDITIONAL(g_object));

   G_OBJECT_CLASS(step_conditional_parent_class)->finalize(g_object);
}

static void step_conditional_class_init(StepConditionalClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = step_conditional_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/sequence_runner.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, StepConditional, content_box);
}

static void step_conditional_init(StepConditional *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
}

StepConditional *step_conditional_new(StepExecutable *parent_sequence,
                                      StepExecutable *start_step,
                                      StepExecutable *cond_true_step,
                                      StepExecutable *cond_false_step,
                                      ExecutableConditionCallback_T conditional_callback,
                                      gpointer callback_user_data)
{
   StepConditional *sc;
   sc = g_object_new(STEP_TYPE_CONDITIONAL, NULL);

   sc->parent_sequence = parent_sequence;
   return sc;
}

