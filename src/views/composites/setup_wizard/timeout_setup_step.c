/**
 * @file timeout_setupstep.c
 *
 * @copyright
 * COPYRIGHT 2025
 * DEKA RESEARCH AND DEVELOPMENT CORPORATION
 *
 * Contains confidential and proprietary information which
 * may not be copied, disclosed or used by others except as expressly
 * authorized in writing by DEKA Research & Development Corporation.
 *
 * @critical <Minor, Moderate, Major Single-Point, Major Multi-Point>
 * @brief <brief description of the file>
 * @subsystem <subsystem name>
 */

#include "timeout_setup_step.h"

struct _TimeoutSetupStep
{
   GtkBox         parent;
   GtkImage       *img_step_bullet;
   GtkLabel       *lbl_step_description;
   GtkProgressBar *pbar_step_countdown;
   GtkLabel       *lbl_step_countdown;

   guint          countdown;
   guint          curr_count;
   gboolean       running;

   void           (*on_timeout_expired)(gpointer user_data);
   gpointer       callback_user_data;
};

G_DEFINE_TYPE(TimeoutSetupStep, timeout_setup_step, GTK_TYPE_BOX)

static void timeout_setup_step_finalize(GObject *self);

static gboolean updateTimeoutProgressLabel(gpointer user_data);

static void timeout_setup_step_update_timeout_label(TimeoutSetupStep *self);

void timeout_setup_step_execute(TimeoutSetupStep *self)
{
   if (!self->running)
   {
      self->running = TRUE;
      self->curr_count = self->countdown;
      timeout_setup_step_update_timeout_label(self);
      gdk_threads_add_timeout_seconds(1, (GSourceFunc)updateTimeoutProgressLabel, (gpointer)self);
   }
   else
   {
      g_print("STEP(%s) is already running\n", gtk_label_get_text(self->lbl_step_description));
   }

}

static void timeout_setup_step_class_init(TimeoutSetupStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = timeout_setup_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/timeout_setup_step.glade");
   gtk_widget_class_bind_template_child(widget_class, TimeoutSetupStep, img_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, TimeoutSetupStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, TimeoutSetupStep, pbar_step_countdown);
   gtk_widget_class_bind_template_child(widget_class, TimeoutSetupStep, lbl_step_countdown);
}

static void timeout_setup_step_init(TimeoutSetupStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* timeout_setup_step_new(const gchar *step_description,
                                  guint countdown,
                                  OnTimoutExpiredCallback_T on_timeout,
                                  gpointer callback_user_data)
{
   TimeoutSetupStep *tout;
   tout = g_object_new(TIMEOUT_TYPE_SETUP_STEP, NULL);

   tout->running = FALSE;
   tout->countdown = countdown;
   tout->on_timeout_expired = on_timeout;
   tout->callback_user_data = callback_user_data;
   gtk_label_set_text(tout->lbl_step_description, step_description);

   return GTK_WIDGET(tout);
}

static void timeout_setup_step_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(TIMEOUT_IS_SETUP_STEP(self));

   G_OBJECT_CLASS (timeout_setup_step_parent_class)->finalize (self);
}


static gboolean updateTimeoutProgressLabel(gpointer user_data)
{
   TimeoutSetupStep *self = TIMEOUT_SETUP_STEP(user_data);

   g_print("%s: %d -- %d\n", __func__, self->countdown, self->curr_count);
   self->curr_count--;

   if (self->curr_count > 0)
   {
      // Timeout continuing, update the countdown text
      if (GTK_IS_LABEL(self->lbl_step_countdown))
      {
         timeout_setup_step_update_timeout_label(self);
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
      timeout_setup_step_update_timeout_label(self);
      self->on_timeout_expired(self->callback_user_data);
      return G_SOURCE_REMOVE;
   }
}

static void timeout_setup_step_update_timeout_label(TimeoutSetupStep *self)
{
   char buf[64];
   snprintf(buf, sizeof(buf), "%ds", self->curr_count);
   gtk_label_set_text(GTK_LABEL(self->lbl_step_countdown), buf);
}