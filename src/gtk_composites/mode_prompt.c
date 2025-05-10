/**
 * Created by french on 10/5/24.
 * @brief
 */
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "utils/logging.h"
#include "mode_prompt.h"

struct _ModePrompt
{
   GtkBox         parent;

   GtkEntry       *entry_to_validate;
   GtkCheckButton *ckbtn_enable_service;

   GtkButton      *btn_validate_and_submit;
   GtkButton      *btn_cancel;

   gboolean       (*validate_and_submit_callback) (const gchar *text_to_validate);

   gboolean       sensitive;
   gboolean       valid;
};

G_DEFINE_TYPE(ModePrompt, mode_prompt, GTK_TYPE_BOX )

enum
{
   PROP_0, // Reserved for GObject
   PROP_SENSITIVE,
   PROP_VALID,
   N_MODE_PROMPT_PROPERTIES
};
static GParamSpec *mode_prompt_properties[N_MODE_PROMPT_PROPERTIES] = {NULL, };
static void mode_prompt_finalize( GObject *self );

static void mode_prompt_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   ModePrompt *self = MODE_PROMPT( object );

   switch( prop_id ) {
      case PROP_SENSITIVE:
         mode_prompt_set_sensitive(self, g_value_get_boolean( value ) );
         break;
      case PROP_VALID:
         mode_prompt_set_valid(self, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void mode_prompt_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   ModePrompt *self = MODE_PROMPT( object );

   switch( prop_id ) {
      case PROP_SENSITIVE:
         g_value_set_boolean( value, mode_prompt_get_sensitive( self ) );
         break;
      case PROP_VALID:
         g_value_set_boolean( value, mode_prompt_get_valid( self ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void on_ckbtn_enable_service_toggled(GtkToggleButton *opt_switch, gpointer user_data)
{
   ModePrompt *self = MODE_PROMPT(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   logging_llprintf(LOGLEVEL_INFO, "%s: %s", __func__, self->ckbtn_enable_service ? "modify-ing/overriding" : "disabled entry");

   gboolean mod_over = gtk_toggle_button_get_active(opt_switch);

   gtk_widget_set_sensitive(GTK_WIDGET(self->entry_to_validate), mod_over);
}

static void on_btn_validate_and_submit_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ModePrompt *self = MODE_PROMPT(user_data);
   gboolean valid_input = (* self->validate_and_submit_callback)(gtk_entry_get_text(self->entry_to_validate));

   mode_prompt_set_valid(self, valid_input);

   if (mode_prompt_get_valid(self))
   {
      // What if the "validation_and_submit_callback" destroys "self"?
      gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_to_validate)), "error");
      gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
   }
   else
   {
      gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_to_validate)), "error");
      logging_llprintf(LOGLEVEL_DEBUG, "%s: Invalid input", __func__ );
      gtk_widget_grab_focus(GTK_WIDGET(self->entry_to_validate));
   }
}

static void on_btn_cancel_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ModePrompt *self = MODE_PROMPT(user_data);
   /** (* self->validate_and_submit_callback)(GTK_RESPONSE_REJECT); */
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
}

static void mode_prompt_class_init(ModePromptClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = mode_prompt_finalize;

   gobject_class->get_property = mode_prompt_get_property;
   gobject_class->set_property = mode_prompt_set_property;
   mode_prompt_properties[PROP_SENSITIVE] = g_param_spec_boolean("sensitive",
                                                          "sensitive",
                                                          "sensitive",
                                                          TRUE,
                                                          G_PARAM_READWRITE );
   mode_prompt_properties[PROP_VALID] = g_param_spec_boolean("valid",
                                                                     "valid",
                                                                     "valid",
                                                                     TRUE,
                                                                     G_PARAM_READWRITE );

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/mode_prompt");

   // This could be a component/composite interface
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, entry_to_validate);
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, ckbtn_enable_service);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_ckbtn_enable_service_toggled", (GCallback)on_ckbtn_enable_service_toggled);

   gtk_widget_class_bind_template_child(widget_class, ModePrompt, btn_validate_and_submit);
   gtk_widget_class_bind_template_child(widget_class, ModePrompt, btn_cancel);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_validate_and_submit_clicked", (GCallback)on_btn_validate_and_submit_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_clicked", (GCallback)on_btn_cancel_clicked);
}

static void mode_prompt_init(ModePrompt *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *mode_prompt_new(ValidateFormCallback_T return_callback)
{
   ModePrompt *self;
   self = g_object_new(MODE_TYPE_PROMPT, NULL);

   if (return_callback != NULL)
   {
      self->validate_and_submit_callback = return_callback;
   }
   return GTK_WIDGET(self);
}

static void mode_prompt_finalize( GObject *oSelf )
{
   g_return_if_fail(oSelf != NULL);
   g_return_if_fail(MODE_IS_PROMPT(oSelf));
   ModePrompt *self = MODE_PROMPT(oSelf);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (mode_prompt_parent_class)->finalize (oSelf);
}

gboolean mode_prompt_get_sensitive( ModePrompt *self )
{
   g_return_val_if_fail( MODE_IS_PROMPT( self ), FALSE );
   return self->sensitive;
}
void mode_prompt_set_sensitive( ModePrompt *self, gboolean sensitive )
{
   g_return_if_fail( MODE_IS_PROMPT( self ) );
   self->sensitive = sensitive;
   g_object_notify_by_pspec(G_OBJECT(self), mode_prompt_properties[PROP_SENSITIVE]);
}

gboolean mode_prompt_get_valid( ModePrompt *self)
{
   g_return_val_if_fail( MODE_IS_PROMPT( self ), FALSE );
   return self->valid;
}
void mode_prompt_set_valid( ModePrompt *self, gboolean valid)
{
   g_return_if_fail( MODE_IS_PROMPT( self ) );
   self->valid = valid;
   GtkStyleContext  *context = gtk_widget_get_style_context(GTK_WIDGET(self->entry_to_validate));
   if (self->valid)
   {
      gtk_style_context_remove_class(context, "error");
   }
   else
   {
      gtk_style_context_add_class(context, "error");
   }
   // g_object_notify_by_pspec(G_OBJECT(self), mode_prompt_properties[PROP_VALID]);
}
