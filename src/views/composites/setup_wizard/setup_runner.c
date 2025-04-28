/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_widgets.h"
#include "setup_runner.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

typedef struct
{
   GList          *children;
   GList          *curr_step;
   guint          n_children;
} SetupRunnerPrivate;

struct _SetupRunner
{
   GtkBox   parent;
   GtkBox   *content_box;

   void        (*on_complete)(StepExecutable *parent_sequence, RunModel *run_model);
   gpointer    callback_user_data;

   StepExecutable *parent_sequence;
};

static void setup_runner_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = setup_runner_execute;
}

G_DEFINE_TYPE_WITH_CODE(SetupRunner, setup_runner, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(SetupRunner)
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               setup_runner_executable_interface_init))

void setup_runner_execute(StepExecutable *self, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: SETUP START", __func__);
   SetupRunner *sr = SETUP_RUNNER(self);
   SetupRunnerPrivate *priv = setup_runner_get_instance_private(sr);

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
      if(sr->on_complete != NULL)
      {
         sr->on_complete(STEP_EXECUTABLE(sr), run_model);
      }
      if(sr->parent_sequence != NULL)
      {
         step_executable_execute(sr->parent_sequence, sr->callback_user_data);
      }
   }
}

static void setup_runner_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SETUP_IS_RUNNER(g_object));

   G_OBJECT_CLASS(setup_runner_parent_class)->finalize(g_object);
}

static void setup_runner_class_init(SetupRunnerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = setup_runner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/resource_path/setup_runner.ui");
   gtk_widget_class_bind_template_child_internal(widget_class, SetupRunner, content_box);
}

static void setup_runner_init(SetupRunner *self)
{
   SetupRunnerPrivate *priv = setup_runner_get_instance_private(self);

   gtk_widget_init_template(GTK_WIDGET(self));
   priv->children = NULL;
   priv->curr_step = NULL;
   priv->n_children = 0;
}

SetupRunner *setup_runner_new(StepExecutable *parent_sequence,
                                    ExecutableCallback_T on_setup_complete,
                                    gpointer callback_user_data)
{
   SetupRunner *myself;

   myself = g_object_new(SETUP_TYPE_RUNNER, NULL);
   myself->on_complete = on_setup_complete;
   myself->callback_user_data = callback_user_data;
   myself->parent_sequence = parent_sequence;
   return myself;
}

void setup_runner_add_child(SetupRunner *self, StepExecutable *child)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupRunner *sr = SETUP_RUNNER(self);
   SetupRunnerPrivate *priv = setup_runner_get_instance_private(sr);

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
