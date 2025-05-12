/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "command_step.h"
#include "utils/logging.h"


struct _CommandStep
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;

   GtkButton   *btn_start_next;
   GtkButton   *btn_cancel_skip;

   RunModel             *model;
   RunModelCommandFn_T  command_fn;
   gboolean             cmd_value;
   StepExecutable       *next;
   SetupContextLoader   *loader;
};

static void command_step_executable_interface_init(StepExecutableInterface *iface)
{
   g_return_if_fail(iface != NULL);
   iface->execute = command_step_execute;
   iface->get_next = command_step_get_next;
}

G_DEFINE_TYPE_WITH_CODE(CommandStep, command_step, GTK_TYPE_BOX,
                        G_IMPLEMENT_INTERFACE (STEP_TYPE_EXECUTABLE,
                                               command_step_executable_interface_init))

static void command_step_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(COMMAND_IS_STEP(g_object));

   G_OBJECT_CLASS(command_step_parent_class)->finalize(g_object);
}

static void on_command_step_btn_start_next_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   CommandStep *self = COMMAND_STEP(user_data);
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
//   step_executable_execute(self->next, self->model);
   setup_context_loader_update_context(self->loader, self->next, self->model);
}

static void on_command_step_btn_cancel_skip_clicked(__attribute__((unused)) GtkButton *button,__attribute__((unused)) gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

static void command_step_class_init(CommandStepClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = command_step_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/dekaresearch/pod/command_step");
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_step_bullet);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, lbl_step_description);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, btn_start_next);
   gtk_widget_class_bind_template_child(widget_class, CommandStep, btn_cancel_skip);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_start_next_clicked", (GCallback)on_command_step_btn_start_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_skip_clicked", (GCallback)on_command_step_btn_cancel_skip_clicked);
}

static void command_step_init(CommandStep *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
   self->command_fn = NULL;
   self->model = NULL;
}

CommandStep *command_step_new(const gchar *command_description,
                              RunModel *model,
                              RunModelCommandFn_T run_model_command_fn,
                              gboolean enable,
                              StepExecutable *next,
                              SetupContextLoader *loader)
{
   g_return_val_if_fail(next != NULL, NULL);

   CommandStep *self;
   self = g_object_new(COMMAND_TYPE_STEP, NULL);

   self->model = model;
   self->command_fn = run_model_command_fn;
   self->cmd_value = enable;
   self->loader = loader;
   self->next = next;

   gtk_label_set_text(self->lbl_step_description, command_description);

   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), FALSE);
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_BULLET_FORMAT_STR);

   return self;
}

void command_step_execute(StepExecutable *exec_self, RunModel *model)
{
   CommandStep *self = COMMAND_STEP(exec_self);

   // The "execute" is to enable the start button
   gtk_label_set_markup(self->lbl_step_bullet, BLUE_SELECTED_BULLET_FORMAT_STR);
   gtk_widget_set_sensitive(GTK_WIDGET(self->btn_start_next), TRUE);

   if (self->command_fn != NULL)
   {
      self->command_fn(self->model, self->cmd_value);
   }
}

StepExecutable *command_step_get_next(StepExecutable *exec_self, RunModel *model)
{
   CommandStep *self = COMMAND_STEP(exec_self);
   return self->next;
}