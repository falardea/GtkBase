/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_globals.h"
#include "sequence_runner.h"
#include "step_executable_interface.h"
#include "step_acknowledge.h"
#include "step_timeout.h"
#include "utils/logging.h"

typedef struct
{
   StepExecutable *children[3];
} SequenceRunnerPrivate;

struct _SequenceRunner
{
   GtkBox   parent;
   GtkBox   *content_box;
};

void sequence_runner_execute(StepExecutable *self);

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

void sequence_runner_execute(StepExecutable *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: SEQUENCE START", __func__);
   SequenceRunner *sr = SEQUENCE_RUNNER(self);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(sr);
   // Next
   step_executable_execute(STEP_EXECUTABLE(priv->children[0]));
}

void sequence_runner_first(SequenceRunner *self, gpointer user_data);
void sequence_runner_second(SequenceRunner *self, gpointer user_data);
void sequence_runner_third(SequenceRunner *self, gpointer user_data);

static void sequence_runner_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SEQUENCE_IS_RUNNER(g_object));

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
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   g_type_ensure(STEP_TYPE_TIMEOUT);

   gtk_widget_init_template(GTK_WIDGET(self));

   priv->children[0] = STEP_EXECUTABLE(step_timeout_new("A sample timeout step", 3,
                                                        self, sequence_runner_first, NULL));
   priv->children[1] = STEP_EXECUTABLE(step_timeout_new("A second timeout step", 2,
                                                        self, sequence_runner_second, NULL));
   priv->children[2] = STEP_EXECUTABLE(step_acknowledge_new("An acknowledgement Step",
                                                            "NEXT", self, sequence_runner_third, NULL));

   gtk_box_pack_start(GTK_BOX(self->content_box), GTK_WIDGET(priv->children[0]), TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(self->content_box), GTK_WIDGET(priv->children[1]), TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(self->content_box), GTK_WIDGET(priv->children[2]), TRUE, TRUE, 0);
}

SequenceRunner *sequence_runner_new()
{
   SequenceRunner *myself;

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   myself = g_object_new(SEQUENCE_TYPE_RUNNER, NULL);

   return myself;
}

void sequence_runner_first(SequenceRunner *self, __attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);
   // Next
   step_executable_execute(STEP_EXECUTABLE(priv->children[1]));
}

void sequence_runner_second(__attribute__((unused)) SequenceRunner *self, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   SequenceRunnerPrivate *priv = sequence_runner_get_instance_private(self);
   step_executable_execute(STEP_EXECUTABLE(priv->children[2]));
}

void sequence_runner_third(__attribute__((unused)) SequenceRunner *self, __attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s: DONE", __func__);
}