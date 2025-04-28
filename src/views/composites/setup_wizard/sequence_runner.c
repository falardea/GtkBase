/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_widgets.h"
#include "sequence_runner.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

typedef struct
{
   GList          *children;
   guint          n_children;
} SequenceRunnerPrivate;

struct _SequenceRunner
{
   GtkBox   parent;
   GtkBox   *content_box;

   // void        (*on_complete)(StepExecutable *parent_sequence, RunModel *run_model);
   // gpointer    callback_user_data;
   //
   // StepExecutable *parent_sequence;
};

// The execute function needs to be exposed in order to inject it into children
void sequence_runner_execute(StepExecutable *self, RunModel *run_model);
static StepIterator *sequence_runner_get_iterator(StepExecutable *self);
static void sequence_runner_cancel(StepExecutable *self, RunModel *run_model);

static void sequence_runner_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = sequence_runner_execute;
   iface->get_iterator = sequence_runner_get_iterator;
   iface->cancel = sequence_runner_cancel;
}

G_DEFINE_TYPE_WITH_CODE(SequenceRunner, sequence_runner, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(SequenceRunner)
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               sequence_runner_executable_interface_init))

static void sequence_runner_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SEQUENCE_IS_RUNNER(g_object));
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(SEQUENCE_RUNNER(g_object));
   g_list_free_full(priv->children, g_object_unref);
   G_OBJECT_CLASS(sequence_runner_parent_class)->finalize(g_object);
}

static void sequence_runner_class_init(SequenceRunnerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = sequence_runner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/sequence_runner.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, SequenceRunner, content_box);
}

static void sequence_runner_init(SequenceRunner *self)
{
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);
   gtk_widget_init_template(GTK_WIDGET(self));
   priv->children = NULL;
   priv->n_children = 0;
}

SequenceRunner *sequence_runner_new()
{
   SequenceRunner *myself;

   myself = g_object_new(SEQUENCE_TYPE_RUNNER, NULL);
   return myself;
}

void sequence_runner_add_child(SequenceRunner *self, StepExecutable *child)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);
   priv->children = g_list_append(priv->children, g_object_ref(child));
   priv->n_children++;

   GtkWidget *w_child = GTK_WIDGET(child);
   if(GTK_IS_WIDGET(child))
   {
      gtk_box_pack_end(GTK_BOX(self->content_box), w_child, TRUE, TRUE, 0);
      gtk_widget_set_visible(w_child, (priv->n_children == 1)); // set only the first child visible
   }
}

void sequence_runner_execute(StepExecutable *self, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: SEQUENCE START", __func__);
   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);
   StepIterator *iter = step_iterator_new(priv->children);
   while(step_iterator_has_next(iter))
   {
      StepExecutable *child = step_iterator_next(iter);

      // TODO: Clean this up
      // Grok suggested this RunModel trap, but only checking the phase.  The check is
      // a good idea, but let's also check the error state.
      RUN_MODEL_PHASES phase = run_model_get_run_phase(run_model);
      GError *err = run_model_get_error(run_model);
      if(phase == RM_FAILED || phase == RM_CANCELLED || err != NULL)
      {
         // TODO: maybe have a logger attached to the rm that
         logging_llprintf(LOGLEVEL_DEBUG, "Unexpected sequence termination: phase(%d) - error(%p)", phase, err);
         break;
      }

      step_executable_execute(child, run_model);
      if (GTK_IS_WIDGET(child))
      {
         // gtk_widget_set_visible(GTK_WIDGET(child), TRUE);
         // Grok's way...
         gtk_widget_show(GTK_WIDGET(child));
         gtk_main_iteration_do(FALSE);
      }
   }
   g_object_unref(iter);
}

static StepIterator *sequence_runner_get_iterator(StepExecutable *self)
{
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(SEQUENCE_RUNNER(self));
   return step_iterator_new(priv->children);
}

static void sequence_runner_cancel(StepExecutable *self, RunModel *run_model)
{
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(SEQUENCE_RUNNER(self));
   StepIterator *iter = step_iterator_new(priv->children);
   while(step_iterator_has_next(iter))
   {
      StepExecutable  *child = step_iterator_next(iter);
      step_executable_cancel(child, run_model);
   }
   g_object_unref(iter);
   run_model_set_run_phase(run_model, RM_CANCELLED);
}
