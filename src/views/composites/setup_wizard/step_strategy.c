#include "step_strategy.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

struct _StepStrategy
{
   GObject                       super;
   ExecutableConditionCallback_T condition;
   GList                         *strategies;
   GtkWidget                     *ui_presentation;
};

static void step_strategy_execute(StepExecutable *step, RunModel *run_model);
static void step_strategy_cancel(StepExecutable *step, RunModel *run_model);
static StepIterator *step_strategy_get_iterator(StepExecutable *step);

static void step_strategy_iface_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(iface != NULL);
   iface->execute = step_strategy_execute;
   iface->cancel = step_strategy_cancel;
   iface->get_iterator = step_strategy_get_iterator;
}

G_DEFINE_TYPE_WITH_CODE(StepStrategy, step_strategy, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_strategy_iface_init))

static void step_strategy_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_STRATEGY(g_object));
   StepStrategy *self = STEP_STRATEGY(g_object);
   g_list_free_full(self->strategies, g_object_unref);
   if (self->ui_presentation)
   {
      gtk_widget_destroy(self->ui_presentation);
   }
   G_OBJECT_CLASS(step_strategy_parent_class)->finalize(g_object);
}

static void step_strategy_class_init(StepStrategyClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = step_strategy_finalize;
}

static void step_strategy_init(StepStrategy *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   self->condition = NULL;
   self->strategies= NULL;
   self->ui_presentation = NULL;
}

StepStrategy *step_strategy_new(ExecutableConditionCallback_T condition,
                                GtkWidget *ui_presentation)
{
   StepStrategy *self;
   self = g_object_new(STEP_TYPE_STRATEGY, NULL);

   self->condition = condition;
   self->ui_presentation = ui_presentation;

   return self;
}

void step_strategy_add_strategy(StepStrategy *self, StepExecutable *strategy)
{
   g_return_if_fail(STEP_IS_STRATEGY(self));
   g_return_if_fail(STEP_IS_EXECUTABLE(strategy));
   self->strategies = g_list_append(self->strategies, g_object_ref(strategy));
}

static void step_strategy_execute(StepExecutable *step, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   StepStrategy *self = STEP_STRATEGY(step);
   gsize index = self->condition ? (self->condition(step, run_model) ? 0 : 1) : 0;
   GList *strategy_node = g_list_nth(self->strategies, index);
   if(strategy_node)
   {
      StepExecutable *strategy = STEP_EXECUTABLE(strategy_node->data);
      step_executable_execute(strategy, run_model);
      if(self->ui_presentation)
      {
         gtk_widget_show(self->ui_presentation);
         gtk_main_iteration_do(FALSE);
      }
   }
   else
   {
      logging_llprintf(LOGLEVEL_ERROR, "No strategy for index %zu", index);
   }
}

static StepIterator *step_strategy_get_iterator(StepExecutable *step)
{
   StepStrategy *self = STEP_STRATEGY(step);
   return step_iterator_new(self->strategies);
}

static void step_strategy_cancel(StepExecutable *step, RunModel *run_model)
{
   StepStrategy *self = STEP_STRATEGY(step);
   StepIterator *iter = step_iterator_new(self->strategies);
   while(step_iterator_has_next(iter))
   {
      StepExecutable *strategy = step_iterator_next(iter);
      step_executable_cancel(strategy, run_model);
   }
   g_object_unref(iter);
   run_model_set_run_phase(run_model, RM_CANCELLED);
}
