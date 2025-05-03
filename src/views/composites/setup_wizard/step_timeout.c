/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_timeout.h"
#include "run_model.h"
#include "utils/logging.h"

typedef struct {
   gchar          *step_description;
   guint          countdown;
   guint          curr_count;
   gboolean       running;
   guint          timeout_id; // for removing from g_source on cancel

   // void           (*on_next_when_expired)(StepExecutable *parent_sequence, RunModel *run_model);
   RunModel    *runModel;
} StepTimeoutPrivate;

struct _StepTimeout
{
   GtkBox         super;
   GtkLabel       *lbl_step_bullet;
   GtkLabel       *lbl_step_description;
   GtkProgressBar *pbar_step_countdown;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;
};

static void step_timeout_execute(StepExecutable *step, RunModel *run_model);
static void step_timeout_cancel(StepExecutable *step, RunModel *run_model);

static void step_timeout_update_progress(StepTimeout *self);
static void step_timeout_set_progress_complete(StepTimeout *self);
static gboolean update_timeout_progress_label(gpointer user_data);
void on_step_timeout_btn_start_next_clicked(GtkButton *button, gpointer user_data);
void on_step_timeout_btn_cancel_skip_clicked(GtkButton *button, gpointer user_data);


static void step_timeout_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = step_timeout_execute;
   iface->cancel = step_timeout_cancel;
   iface->get_iterator = NULL; // Leaf
}

G_DEFINE_TYPE_WITH_CODE(StepTimeout, step_timeout, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(StepTimeout)
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_timeout_executable_interface_init))

static void step_timeout_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_TIMEOUT(g_object));

   StepTimeoutPrivate *priv = step_timeout_get_instance_private(STEP_TIMEOUT(g_object));
   g_free(priv->step_description);
   if(priv->timeout_id)
   {
      g_source_remove(priv->timeout_id);
   }
   G_OBJECT_CLASS (step_timeout_parent_class)->finalize (g_object);
}

static void step_timeout_class_init(StepTimeoutClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_timeout_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_timeout.ui");
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, pbar_step_countdown);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_step_timeout_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_step_timeout_btn_cancel_skip_clicked);
}

static void step_timeout_init(StepTimeout *self)
{
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);
   g_type_ensure(STEP_TYPE_EXECUTABLE);
   gtk_widget_init_template(GTK_WIDGET(self));
   priv->step_description = NULL;
   priv->countdown = 0;
   priv->curr_count = 0;
   priv->running = FALSE;
   priv->timeout_id = 0;
}

StepTimeout* step_timeout_new(const gchar *step_description,
                              guint countdown,
                              ExecutableCallback_T on_next,
                              RunModel *runModel)
{
   StepTimeout *self = g_object_new(STEP_TYPE_TIMEOUT, NULL);
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);

   priv->step_description = g_strdup(step_description);
   priv->countdown = countdown;
   // priv->on_next_when_expired = on_next;
   priv->runModel = runModel;

   gtk_label_set_text(self->lbl_step_description, priv->step_description);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);

   char progress_str[64];
   snprintf(progress_str, sizeof(progress_str), "%d s", priv->countdown);
   gtk_progress_bar_set_text(self->pbar_step_countdown, progress_str);
   gtk_progress_bar_set_fraction(self->pbar_step_countdown, 0.0);

   return self;
}

static void step_timeout_update_progress(StepTimeout *self)
{
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);
   char buf[64];
   snprintf(buf, sizeof(buf), "%d s", priv->curr_count);

   gdouble tick_val = (priv->countdown > 0) ? 1.0-(priv->curr_count/(gdouble)priv->countdown) : (gdouble)priv->countdown;

   gtk_progress_bar_set_text(self->pbar_step_countdown, buf);
   gtk_progress_bar_set_fraction(self->pbar_step_countdown, tick_val);
}

static void step_timeout_set_progress_complete(StepTimeout *self)
{
   char buf[64];
   snprintf(buf, sizeof(buf), "%s", "COMPLETE");

   gtk_progress_bar_set_text(self->pbar_step_countdown, buf);
   gtk_progress_bar_set_fraction(self->pbar_step_countdown, 1);
}

static gboolean update_timeout_progress_label(gpointer user_data)
{
   StepTimeout *self = STEP_TIMEOUT(user_data);
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);
   priv->curr_count--;

   if (priv->curr_count > 0)
   {
      step_timeout_update_progress(self);
      return G_SOURCE_CONTINUE;
   }
   else
   {
      priv->running = FALSE;
      priv->timeout_id = 0;
      gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
      gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), TRUE);
      step_timeout_set_progress_complete(self);

      // Grok hallucinated here:
      run_model_set_data(priv->runModel, "timeout_result", g_strdup("completed"), g_free);

      return G_SOURCE_REMOVE;
   }
}

void step_timeout_execute(StepExecutable *step, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   StepTimeout *self = STEP_TIMEOUT(step);
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);
   RUN_MODEL_PHASES phase = run_model_get_run_phase(run_model);
   GError *err = run_model_get_error(run_model);
   if(phase == RM_FAILED || phase == RM_CANCELLED || err)
   {
      return;
   }
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   if (!priv->running)
   {
      priv->running = TRUE;
      priv->curr_count = priv->countdown;
      priv->timeout_id = gdk_threads_add_timeout_seconds(1, (GSourceFunc) update_timeout_progress_label, (gpointer)self);
   }
   else
   {
      g_print("STEP(%s) is already running\n", gtk_label_get_text(self->lbl_step_description));
   }
}

static void step_timeout_cancel(StepExecutable *step, RunModel *run_model)
{
   StepTimeout *self = STEP_TIMEOUT(step);
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);
   if(priv->timeout_id)
   {
      g_source_remove(priv->timeout_id);
      priv->timeout_id = 0;
      priv->running = FALSE;
      gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
      gtk_progress_bar_set_text(self->pbar_step_countdown, "CANCELLED");
      gtk_progress_bar_set_fraction(self->pbar_step_countdown, 0.0);
      run_model_set_run_phase(run_model, RM_CANCELLED);
   }
}

void on_step_timeout_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   StepTimeout *self = STEP_TIMEOUT(user_data);
   StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);

   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);


   // Grok hallucinated here too:
   run_model_set_data(priv->runModel, "timeout_result", g_strdup("skipped"), g_free);

   // if (sto->on_next_when_expired != NULL)
   // {
   //    sto->on_next_when_expired(STEP_EXECUTABLE(sto), sto->callback_user_data);
   // }
}

void on_step_timeout_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   // StepTimeout *self = STEP_TIMEOUT(user_data);
   // StepTimeoutPrivate *priv = step_timeout_get_instance_private(self);

   // step_timeout_cancel(STEP_EXECUTABLE(self), priv->callback_user_data);
}

