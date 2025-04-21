/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_widgets.h"
#include "sequence_runner.h"
#include "step_executable_interface.h"
#include "step_acknowledge.h"
#include "step_timeout.h"
#include "step_sequence.h"
#include "utils/logging.h"

typedef struct
{
   GList          *children;
   GList          *curr_step;
   guint          n_children;
   gboolean       complete;
} SequenceRunnerPrivate;

struct _SequenceRunner
{
   GtkBox   parent;
   GtkBox   *content_box;

   GtkProgressBar *progress_bar_box;
   GtkBox         *sequence_banner;

   void        (*on_complete)(StepExecutable *parent_sequence, RunModel *run_model);
   gpointer    callback_user_data;

   StepExecutable *parent_sequence;
};

static void sequence_runner_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = sequence_runner_execute;
}

G_DEFINE_TYPE_WITH_CODE(SequenceRunner, sequence_runner, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(SequenceRunner)
                              G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                                     sequence_runner_executable_interface_init))

void sequence_runner_execute(StepExecutable *self, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: SEQUENCE START", __func__);
   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);

   if (priv->curr_step != NULL)
   {
      // We have at least one step to execute, execute it
      GList    *pre_step = (priv->curr_step)->prev;
      step_executable_execute(STEP_EXECUTABLE((priv->curr_step)->data), run_model);

      if (pre_step != NULL)
      {
         // Show the next step, if we have one
         gtk_widget_set_visible(GTK_WIDGET(pre_step->data), FALSE);
         gtk_widget_set_visible(GTK_WIDGET((priv->curr_step)->data), TRUE);
      }
      priv->curr_step = (priv->curr_step)->next;
   }
   else
   {
      // We either have zero steps to run, or maybe we've implemented the skippable interface
      if (priv->n_children > 0)
      {
         GList    *first_step = g_list_first(priv->children);
         GList    *last_step = g_list_last(priv->children);
         gtk_widget_set_visible(GTK_WIDGET(last_step->data), FALSE);
         gtk_widget_set_visible(GTK_WIDGET(first_step->data), TRUE);
         priv->curr_step = first_step;
      }
      sr->on_complete(sr->parent_sequence, run_model);
   }
}

static void sequence_runner_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SEQUENCE_IS_RUNNER(g_object));

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

   g_type_ensure(STEP_TYPE_TIMEOUT);
   g_type_ensure(STEP_TYPE_ACKNOWLEDGE);
   g_type_ensure(STEP_TYPE_SEQUENCE);

   gtk_widget_init_template(GTK_WIDGET(self));
   priv->children = NULL;
   priv->curr_step = NULL;
   priv->n_children = 0;
   priv->complete = FALSE;
}

SequenceRunner *sequence_runner_new(StepExecutable *parent_sequence,
                                    ExecutableCallback_T on_sequence_complete,
                                    gpointer callback_user_data)
{
   SequenceRunner *myself;

   myself = g_object_new(SEQUENCE_TYPE_RUNNER, NULL);
   myself->on_complete = on_sequence_complete;
   myself->callback_user_data = callback_user_data;
   myself->parent_sequence = parent_sequence;
   return myself;
}

void sequence_runner_add_child(SequenceRunner *self, StepExecutable *child)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);

   priv->children = g_list_append(priv->children, child);
   priv->n_children++;
   gtk_box_pack_end(GTK_BOX(sr->content_box), GTK_WIDGET(child), TRUE, TRUE, 0);
   gtk_widget_set_visible(GTK_WIDGET(child), (priv->n_children) ? FALSE : TRUE); // set only the first child visible

   if (g_list_length(priv->children) == 1)
   {
      priv->curr_step = g_list_first(priv->children);
      gtk_widget_show(GTK_WIDGET(child));
   }
}
