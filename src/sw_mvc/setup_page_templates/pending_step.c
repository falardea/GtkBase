/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "pending_step.h"
#include "utils/logging.h"

struct _PendingStep
{
   GtkBox      super;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_cancel_skip;

   gboolean             process_completed;

   RunModel             *model;
   gchar                *bound_property_str;
   StepExecutable       *next_on_success;
   StepExecutable       *next_on_failure;
   StepExecutable       *next;
   SetupContextLoader   *loader;
};

enum {
   PROP_O,
   PENDING_STEP_PROCESS_COMPLETED,
   N_PENDING_STEP_PROPERTIES
};

static void pending_step_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = pending_step_execute;
   iface->get_next = pending_step_get_next;
}

G_DEFINE_TYPE_WITH_CODE(PendingStep, pending_step, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               pending_step_executable_interface_init))

static void pending_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PENDING_IS_STEP(g_object));
   PendingStep *self = PENDING_STEP(g_object);
   g_free(self->bound_property_str);
   G_OBJECT_CLASS(pending_step_parent_class)->finalize(g_object);
}

void on_pending_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void pending_step_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   PendingStep *self = PENDING_STEP(object);
   switch(prop_id){
      case PENDING_STEP_PROCESS_COMPLETED:
         pending_step_set_process_completed(self, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void pending_step_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   PendingStep *self = PENDING_STEP(object);
   switch(prop_id)
   {
      case PENDING_STEP_PROCESS_COMPLETED:
         g_value_set_boolean( value, pending_step_get_process_completed(self));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *pending_step_properties[N_PENDING_STEP_PROPERTIES] = {NULL, };

static void pending_step_class_init(PendingStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = pending_step_finalize;

   gobject_class->set_property = pending_step_set_property;
   gobject_class->get_property = pending_step_get_property;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/pending_step");
   gtk_widget_class_bind_template_child(widget_class, PendingStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, PendingStep, btn_cancel_skip);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_pending_step_btn_cancel_skip_clicked);

   pending_step_properties[PENDING_STEP_PROCESS_COMPLETED] = g_param_spec_boolean("pending-process-complete",
                                                                    "Pending process complete",
                                                                    "The pending process has completed",
                                                                    FALSE,
                                                                    G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, N_PENDING_STEP_PROPERTIES, pending_step_properties);
}

static void pending_step_init(PendingStep *self)
{
   g_return_if_fail(PENDING_IS_STEP(self));
   gtk_widget_init_template(GTK_WIDGET(self));
   self->next_on_success = NULL;
   self->next_on_failure = NULL;
   self->next = NULL;
}

PendingStep *pending_step_new(const gchar *pending_description,
                              RunModel *model,
                              const gchar *bound_model_prop,
                              StepExecutable *on_success,
                              StepExecutable *on_failure,
                              SetupContextLoader *loader)
{
   PendingStep *self;
   self = g_object_new(PENDING_TYPE_STEP, NULL);

   if (pending_description != NULL)
   {
      gtk_label_set_text(self->lbl_step_description, pending_description);
   }
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_cancel_skip), FALSE);

   self->process_completed = FALSE;
   self->bound_property_str = g_strdup(bound_model_prop);
   self->model = model;
   self->next_on_success = on_success;
   self->next_on_failure = on_failure;
   self->loader = loader;

   return self;
}

void pending_step_set_process_completed(PendingStep *self, gboolean enable)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   g_return_if_fail(PENDING_IS_STEP(self));
   self->process_completed = enable;
   setup_context_loader_update_context(self->loader, self->next, self->model);
}
gboolean pending_step_get_process_completed(PendingStep *self)
{
   g_return_val_if_fail(PENDING_IS_STEP(self), FALSE);
   return self->process_completed;
}

void pending_step_execute(StepExecutable *exec_self, RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   PendingStep *self = PENDING_STEP(exec_self);

   self->process_completed = FALSE; // reset on execute to make re-entrant?
   GValue low_mem = G_VALUE_INIT;
   g_value_init(&low_mem, G_TYPE_BOOLEAN);
   g_object_get_property(G_OBJECT(model), self->bound_property_str, &low_mem);
   gboolean has_low_mem = g_value_get_boolean(&low_mem);
   g_value_unset(&low_mem);

   //   g_object_bind_property(self->model, self->bound_property_str, self, "pending-process-complete", G_BINDING_DEFAULT);

   self->next = (has_low_mem) ? self->next_on_failure : self->next_on_success;
   pending_step_set_process_completed(self, TRUE);

}

StepExecutable *pending_step_get_next(StepExecutable *exec_self, RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   PendingStep *self = PENDING_STEP(exec_self);
   return self->next;
}

void pending_step_set_next_on_success(PendingStep *self, StepExecutable *next_on_success)
{
   self->next_on_success = next_on_success;
}

void pending_step_set_next_on_failure(PendingStep *self, StepExecutable *next_on_failure)
{
   self->next_on_failure = next_on_failure;
}