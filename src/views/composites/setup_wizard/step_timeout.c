/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_timeout.h"
#include "run_model.h"
#include "utils/logging.h"

struct _StepTimeout
{
   GtkBox         parent;
   GtkLabel       *lbl_step_bullet;
   GtkLabel       *lbl_step_description;
   GtkProgressBar *pbar_step_countdown;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;

   guint          countdown;
   guint          curr_count;
   gboolean       running;

   void           (*on_next_when_expired)(StepExecutable *parent_sequence, RunModel *run_model);
   gpointer       callback_user_data;

   StepExecutable *parent_sequence;
};

static void step_timeout_update_progress(StepTimeout *self);
static void step_timeout_set_progress_complete(StepTimeout *self);
static gboolean updateTimeoutProgressLabel(gpointer user_data);

void step_timeout_execute(StepExecutable *self,__attribute__((unused)) RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   StepTimeout *st = STEP_TIMEOUT(self);

   gtk_label_set_markup(st->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(st->btn_start_next), FALSE);
   if (!st->running)
   {
      st->running = TRUE;
      st->curr_count = st->countdown;
      gdk_threads_add_timeout_seconds(1, (GSourceFunc)updateTimeoutProgressLabel, (gpointer)st);
   }
   else
   {
      g_print("STEP(%s) is already running\n", gtk_label_get_text(st->lbl_step_description));
   }
}

void on_step_timeout_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   StepTimeout *sto = STEP_TIMEOUT(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(sto->btn_start_next), FALSE);
   gtk_label_set_markup(sto->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   sto->on_next_when_expired(STEP_EXECUTABLE(sto->parent_sequence), sto->callback_user_data);
}

void on_step_timeout_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void step_timeout_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = step_timeout_execute;
}

G_DEFINE_TYPE_WITH_CODE(StepTimeout, step_timeout, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               step_timeout_executable_interface_init))

static void step_timeout_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_TIMEOUT(g_object));

   G_OBJECT_CLASS (step_timeout_parent_class)->finalize (g_object);
}

static void step_timeout_class_init(StepTimeoutClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_timeout_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/step_timeout.ui");
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
   g_type_ensure(STEP_TYPE_EXECUTABLE);

   gtk_widget_init_template(GTK_WIDGET(self));
}

StepTimeout* step_timeout_new(const gchar *step_description,
                              guint countdown,
                              StepExecutable *parent_sequence,
                              ExecutableCallback_T on_next,
                              gpointer callback_user_data)
{
   StepTimeout *st;
   st = g_object_new(STEP_TYPE_TIMEOUT, NULL);

   st->running = FALSE;
   st->countdown = countdown;
   st->on_next_when_expired = on_next;
   st->callback_user_data = callback_user_data;
   st->parent_sequence = parent_sequence;

   gtk_label_set_text(st->lbl_step_description, step_description);
   gtk_label_set_markup(st->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(st->btn_start_next), FALSE);

   char progress_str[64];
   snprintf(progress_str, sizeof(progress_str), "%d s", st->countdown);
   gtk_progress_bar_set_text(st->pbar_step_countdown, progress_str);
   gtk_progress_bar_set_fraction(st->pbar_step_countdown, 0.0);

   return st;
}

static gboolean updateTimeoutProgressLabel(gpointer user_data)
{
   StepTimeout *self = STEP_TIMEOUT(user_data);

   g_print("%s: %d/%d\n", __func__, self->curr_count, self->countdown);
   self->curr_count--;

   if (self->curr_count > 0)
   {
      // Timeout continuing, update the countdown text
      if (GTK_IS_PROGRESS_BAR(self->pbar_step_countdown))
      {
         step_timeout_update_progress(self);
      }
      else
      {
         g_print("%s: self->pbar_step_countdown is not a label\n", __func__);
      }
      return G_SOURCE_CONTINUE;
   }
   else
   {
      self->running = FALSE;
      gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);
      gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), TRUE);
      step_timeout_set_progress_complete(self);
      return G_SOURCE_REMOVE;
   }
}

static void step_timeout_update_progress(StepTimeout *self)
{
   char buf[64];
   snprintf(buf, sizeof(buf), "%d s", self->curr_count);

   gdouble tick_val = (self->countdown > 0) ? 1.0-(self->curr_count/(gdouble)self->countdown) : (gdouble)self->countdown;

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
