/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_timeout.h"

struct _StepTimeout
{
   GtkBox         parent;
   GtkImage       *img_step_bullet;
   GtkLabel       *lbl_step_description;
   GtkProgressBar *pbar_step_countdown;
   GtkLabel       *lbl_step_countdown;

   guint          countdown;
   guint          curr_count;
   gboolean       running;

   void           (*on_timeout_expired)(SequenceRunner *parent_sequence, gpointer user_data);
   gpointer       callback_user_data;

   SequenceRunner *parent_sequence;
};

G_DEFINE_TYPE(StepTimeout, step_timeout, GTK_TYPE_BOX)

static void step_timeout_finalize(GObject *g_object);

static gboolean updateTimeoutProgressLabel(gpointer user_data);

static void step_timeout_update_timeout_label(StepTimeout *self);

void step_timeout_execute(StepTimeout *self)
{
   if (!self->running)
   {
      self->running = TRUE;
      self->curr_count = self->countdown;
      step_timeout_update_timeout_label(self);
      gdk_threads_add_timeout_seconds(1, (GSourceFunc)updateTimeoutProgressLabel, (gpointer)self);
   }
   else
   {
      g_print("STEP(%s) is already running\n", gtk_label_get_text(self->lbl_step_description));
   }

}

static void step_timeout_class_init(StepTimeoutClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = step_timeout_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/step_timeout.ui");
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, img_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, pbar_step_countdown);
   gtk_widget_class_bind_template_child(widget_class, StepTimeout, lbl_step_countdown);
}

static void step_timeout_init(StepTimeout *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

StepTimeout* step_timeout_new(const gchar *step_description,
                                         guint countdown,
                                         SequenceRunner *parent_sequence,
                                         SequenceCallback_T on_timeout,
                                         gpointer callback_user_data)
{
   StepTimeout *tout;
   tout = g_object_new(STEP_TYPE_TIMEOUT, NULL);

   tout->running = FALSE;
   tout->countdown = countdown;
   tout->on_timeout_expired = on_timeout;
   tout->callback_user_data = callback_user_data;
   tout->parent_sequence = parent_sequence;

   gtk_label_set_text(tout->lbl_step_description, step_description);

   return tout;
}

static void step_timeout_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(STEP_IS_TIMEOUT(g_object));

   G_OBJECT_CLASS (step_timeout_parent_class)->finalize (g_object);
}

static gboolean updateTimeoutProgressLabel(gpointer user_data)
{
   StepTimeout *self = STEP_TIMEOUT(user_data);

   g_print("%s: %d -- %d\n", __func__, self->countdown, self->curr_count);
   self->curr_count--;

   if (self->curr_count > 0)
   {
      // Timeout continuing, update the countdown text
      if (GTK_IS_LABEL(self->lbl_step_countdown))
      {
         step_timeout_update_timeout_label(self);
      }
      else
      {
         g_print("%s: self->lbl_step_countdown is not a label\n", __func__);
      }
      return G_SOURCE_CONTINUE;
   }
   else
   {
      self->running = FALSE;
      step_timeout_update_timeout_label(self);
      self->on_timeout_expired(self->parent_sequence, self->callback_user_data);
      return G_SOURCE_REMOVE;
   }
}

static void step_timeout_update_timeout_label(StepTimeout *self)
{
   char buf[64];
   snprintf(buf, sizeof(buf), "%ds", self->curr_count);
   gtk_label_set_text(GTK_LABEL(self->lbl_step_countdown), buf);
}