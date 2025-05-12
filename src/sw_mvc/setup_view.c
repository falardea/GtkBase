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

SetupViewer *setup_viewer_new(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupViewer *self;
   self = g_object_new(SETUP_TYPE_VIEWER, NULL);

   self->model = model;

   return self;
}

void on_btn_user_choice_cancel_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_user_choice_next_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_state_observer_cancel_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_state_observer_next_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_value_input_cancel_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
void on_btn_value_input_next_clicked(GtkButton *button, SetupViewer *self)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}
