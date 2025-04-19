/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_globals.h"
#include "sequence_runner.h"
#include "step_executable_interface.h"
#include "step_acknowledge.h"
#include "step_timeout.h"
#include "step_sequence.h"
#include "utils/logging.h"

#define SEQUENCE_RUNNER_N_STEPS 4
typedef struct
{
   StepExecutable *children[SEQUENCE_RUNNER_N_STEPS];
   size_t         n_children;
   guint          curr_step;
} SequenceRunnerPrivate;

struct _SequenceRunner
{
   GtkBox   parent;
   GtkBox   *content_box;

   void        (*on_complete)(StepExecutable *parent_sequence, gpointer user_data);
   gpointer    callback_user_data;
};

void sequence_runner_execute(StepExecutable *self, gpointer user_data);

static void sequence_runner_executable_interface_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(iface != NULL);
   iface->execute = sequence_runner_execute;
}

G_DEFINE_TYPE_WITH_CODE(SequenceRunner, sequence_runner, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(SequenceRunner)
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               sequence_runner_executable_interface_init))

void sequence_runner_execute(StepExecutable *self, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: SEQUENCE START", __func__);
   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);

   if (priv->curr_step == 0)
   {
      // We're on the first step or we
      step_executable_execute(STEP_EXECUTABLE(priv->children[priv->curr_step]), user_data);
      priv->curr_step++;
   }
   else if (priv->curr_step < priv->n_children)
   {
      gtk_widget_set_visible(GTK_WIDGET(priv->children[priv->curr_step-1]), FALSE);
      gtk_widget_set_visible(GTK_WIDGET(priv->children[priv->curr_step]), TRUE);
      step_executable_execute(STEP_EXECUTABLE(priv->children[priv->curr_step]), user_data);
      priv->curr_step++;
   }
   else
   {
      gtk_widget_set_visible(GTK_WIDGET(priv->children[priv->curr_step-1]), FALSE);
      priv->curr_step = 0;
      gtk_widget_set_visible(GTK_WIDGET(priv->children[priv->curr_step]), TRUE);
      sr->on_complete(NULL, user_data);
   }
}

static void sequence_runner_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SEQUENCE_IS_RUNNER(g_object));

   // SequenceRunner *sr = SEQUENCE_RUNNER(g_object);
   // SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);

   G_OBJECT_CLASS(sequence_runner_parent_class)->finalize(g_object);
}

static void sequence_runner_class_init(SequenceRunnerClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = sequence_runner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/sequence_runner.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, SequenceRunner, content_box);
}

static void sequence_runner_init(SequenceRunner *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);

   gtk_widget_init_template(GTK_WIDGET(self));

   priv->curr_step = 0;
   priv->n_children = 0;
   sequence_runner_add_child(self,
                             STEP_EXECUTABLE(step_sequence_new(
                                   "Routine Start",
                                   "START",
                                   self,
                                   sequence_runner_execute,
                                   NULL)));
   sequence_runner_add_child(self, STEP_EXECUTABLE(step_timeout_new("A sample timeout step", 3, self, sequence_runner_execute, NULL)));
   sequence_runner_add_child(self, STEP_EXECUTABLE(step_timeout_new("A second timeout step", 2, self, sequence_runner_execute, NULL)));
   sequence_runner_add_child(self, STEP_EXECUTABLE(step_acknowledge_new("An acknowledgement Step", "NEXT", self, sequence_runner_execute, NULL)));
}

SequenceRunner *sequence_runner_new(ExecutableCallback_T on_sequence_complete,
                                    gpointer callback_user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SequenceRunner *myself;

   myself = g_object_new(SEQUENCE_TYPE_RUNNER, NULL);

   myself->on_complete = on_sequence_complete;
   myself->callback_user_data = callback_user_data;

   return myself;
}

void sequence_runner_add_child(SequenceRunner *self, StepExecutable *child)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);

   priv->children[priv->n_children] = child;
   gtk_box_pack_end(GTK_BOX(sr->content_box), GTK_WIDGET(child), TRUE, TRUE, 0);

   gtk_widget_set_visible(GTK_WIDGET(child), (priv->n_children) ? FALSE : TRUE); // set only the first child visible

   priv->n_children++;
}