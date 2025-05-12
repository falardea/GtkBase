#include "setup_view.h"
#include "utils/logging.h"

struct _SetupViewer
{
   GtkBox   super;
   GtkStack *page_stack;

   GtkBox      *user_choice;
   GtkLabel    *lbl_user_choice;
   GtkButton   *btn_user_choice_cancel;
   GtkButton   *btn_user_choice_next;

   GtkBox      *state_observer;
   GtkLabel    *lbl_state_observer;
   GtkButton   *btn_state_observer_cancel;
   GtkButton   *btn_state_observer_next;

   GtkBox      *value_input;
   GtkLabel    *lbl_value_input;
   GtkButton   *btn_value_input_cancel;
   GtkButton   *btn_value_input_next;
   GtkLabel    *lbl_entry_description;
   GtkEntry    *entry_value;
   GtkLabel    *lbl_entry_units;

   RunModelSetterCallback_T   *on_next_callback; // (callback)(RunModel, user_data)
   RunModelSetterCallback_T   *on_back_callback;
   gpointer    next_user_data;
   gpointer    cancel_user_data;

   RunModel *model;
};

G_DEFINE_TYPE(SetupViewer, setup_viewer, GTK_TYPE_BOX)

static void setup_viewer_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   G_OBJECT_CLASS(setup_viewer_parent_class)->finalize(g_object);
}

void on_btn_user_choice_cancel_clicked(GtkButton *button, SetupViewer *self);
void on_btn_user_choice_next_clicked(GtkButton *button, SetupViewer *self);
void on_btn_state_observer_cancel_clicked(GtkButton *button, SetupViewer *self);
void on_btn_state_observer_next_clicked(GtkButton *button, SetupViewer *self);
void on_btn_value_input_cancel_clicked(GtkButton *button, SetupViewer *self);
void on_btn_value_input_next_clicked(GtkButton *button, SetupViewer *self);

void setup_viewer_set_setup_complete(SetupViewer *self, RunModel *model);
void setup_viewer_set_setup_failed(SetupViewer *self, RunModel *model);

static void setup_viewer_class_init(SetupViewerClass *klass)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = setup_viewer_finalize;
   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/setup_viewer");
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, page_stack);

   gtk_widget_class_bind_template_child(widget_class, SetupViewer, user_choice);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, lbl_user_choice);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_user_choice_cancel);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_user_choice_next);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, state_observer);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, lbl_state_observer);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_state_observer_cancel);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_state_observer_next);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, value_input);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, lbl_value_input);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_value_input_cancel);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, btn_value_input_next);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, lbl_entry_description);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, entry_value);
   gtk_widget_class_bind_template_child(widget_class, SetupViewer, lbl_entry_units);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_user_choice_cancel_clicked", (GCallback)on_btn_user_choice_cancel_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_user_choice_next_clicked", (GCallback)on_btn_user_choice_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_state_observer_cancel_clicked", (GCallback)on_btn_state_observer_cancel_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_state_observer_next_clicked", (GCallback)on_btn_state_observer_next_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_value_input_cancel_clicked", (GCallback)on_btn_value_input_cancel_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_value_input_next_clicked", (GCallback)on_btn_value_input_next_clicked);
}

static void setup_viewer_init(SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   gtk_widget_init_template(GTK_WIDGET(self));
}

gboolean setup_viewer_step_change_listener(RunModel *model, RUN_SETUP_STEPS step, gpointer user_data);

SetupViewer *setup_viewer_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupViewer *self;
   self = g_object_new(SETUP_TYPE_VIEWER, NULL);
   self->model = model;
   self->on_next_callback = NULL;
   self->on_back_callback = NULL;
   self->next_user_data = NULL;
   self->cancel_user_data = NULL;

   g_signal_connect (G_OBJECT(model), RUN_MODEL_SETUP_STEP_CHANGE_SIGNAL_STR, G_CALLBACK(setup_viewer_step_change_listener), self);
   return self;
}

void on_btn_user_choice_cancel_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_user_choice_next_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_state_observer_cancel_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_state_observer_next_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_value_input_cancel_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   run_model_set_last_completed_step(self->model, RUN_SETUP_FAILED);
}
void on_btn_value_input_next_clicked(__attribute__((unused)) GtkButton *button, SetupViewer *self)
{
   g_return_if_fail(self != NULL);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   run_model_set_last_completed_step(self->model, RUN_SETUP_COMPLETE);
}

gboolean setup_viewer_step_change_listener(RunModel *model, RUN_SETUP_STEPS step, gpointer user_data)
{
   // This could be a broadcast to all children in the view to update based on a model change
   SetupViewer *self = SETUP_VIEWER(user_data);

   switch(step)
   {
      case RUN_SETUP_UNINITIALIZED:
      case RUN_SETUP_MODE_SELECTED:
      case RUN_SETUP_MEMCHECK_COMPLETE:
      case RUN_SETUP_INTERMEDIATE_STEPS:
         logging_llprintf(LOGLEVEL_DEBUG, "%s: past mode select", __func__);
         gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->value_input));
         break;
      case RUN_SETUP_COMPLETE:
         setup_viewer_set_setup_complete(self, model);
         gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->user_choice));
         break;
      case RUN_SETUP_FAILED:
      default:
         setup_viewer_set_setup_failed(self, model);
         gtk_stack_set_visible_child(self->page_stack, GTK_WIDGET(self->user_choice));
         break;
   }
   return G_SOURCE_REMOVE;
}

void setup_viewer_set_setup_complete(SetupViewer *self, RunModel *model)
{
   gtk_label_set_text(self->lbl_user_choice, "Setup Complete");
   gtk_widget_hide(GTK_WIDGET(self->btn_user_choice_cancel));
   gtk_widget_hide(GTK_WIDGET(self->btn_user_choice_next));
}
void setup_viewer_set_setup_failed(SetupViewer *self, RunModel *model)
{
   gtk_label_set_text(self->lbl_user_choice, "Setup Failed");
   gtk_widget_hide(GTK_WIDGET(self->btn_user_choice_cancel));
   gtk_widget_show(GTK_WIDGET(self->btn_user_choice_next));
   gtk_button_set_label(self->btn_user_choice_next, "Shutdown");
}