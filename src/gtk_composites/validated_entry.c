/**
 * Created by french on 10/5/24.
 * @brief
 */
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "utils/logging.h"
#include "validated_entry.h"

struct _ValidatedEntry
{
   GtkBox         parent;
   GtkLabel       *title_label;
   GtkLabel       *message_label;

   GtkLabel       *entry_label;
   GtkEntry       *entry_to_validate;
   GtkCheckButton *chckbtn_modify_override;

   GtkButton      *btn_validate_and_submit;
   GtkButton      *btn_cancel;
   gboolean       (*validate_and_submit_callback) (const gchar *text_to_validate);
   gchar          *default_text_fallback;
   gboolean       sensitive;
   gboolean       valid;
};

G_DEFINE_TYPE(ValidatedEntry, validated_entry, GTK_TYPE_BOX )

enum
{
   PROP_0, // Reserved for GObject
   PROP_SENSITIVE,
   PROP_VALID,
   PROP_DEFAULT_TEXT_FALLBACK,
   N_VALIDATED_ENTRY_PROPERTIES
};
static GParamSpec *validated_entry_properties[N_VALIDATED_ENTRY_PROPERTIES] = {NULL, };
static void validated_entry_finalize( GObject *self );

static void validated_entry_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   ValidatedEntry *self = VALIDATED_ENTRY( object );

   switch( prop_id ) {
      case PROP_DEFAULT_TEXT_FALLBACK:
         validated_entry_set_default_text_fallback( self, g_value_get_string( value ) );
         break;
      case PROP_SENSITIVE:
         validated_entry_set_sensitive(self, g_value_get_boolean( value ) );
         break;
      case PROP_VALID:
         validated_entry_set_valid(self, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void validated_entry_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   ValidatedEntry *self = VALIDATED_ENTRY( object );

   switch( prop_id ) {
      case PROP_DEFAULT_TEXT_FALLBACK:
         g_value_set_string( value, validated_entry_get_default_text_fallback( self ) );
         break;
      case PROP_SENSITIVE:
         g_value_set_boolean( value, validated_entry_get_sensitive( self ) );
         break;
      case PROP_VALID:
         g_value_set_boolean( value, validated_entry_get_valid( self ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void on_chckbtn_modify_override_toggled(GtkToggleButton *opt_switch, gpointer user_data)
{
   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   logging_llprintf(LOGLEVEL_INFO, "%s: %s", __func__, self->chckbtn_modify_override ? "modify-ing/overriding" : "disabled entry");

   gboolean mod_over = gtk_toggle_button_get_active(opt_switch);

   gtk_widget_set_sensitive(GTK_WIDGET(self->entry_to_validate), mod_over);
}

static void on_btn_validate_and_submit_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   gboolean valid_input = (* self->validate_and_submit_callback)(gtk_entry_get_text(self->entry_to_validate));

   validated_entry_set_valid(self, valid_input);

   if (validated_entry_get_valid(self))
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

   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   /** (* self->validate_and_submit_callback)(GTK_RESPONSE_REJECT); */
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
}

static void validated_entry_class_init(ValidatedEntryClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = validated_entry_finalize;

   gobject_class->get_property = validated_entry_get_property;
   gobject_class->set_property = validated_entry_set_property;
   validated_entry_properties[PROP_SENSITIVE] = g_param_spec_boolean("sensitive",
                                                          "sensitive",
                                                          "sensitive",
                                                          TRUE,
                                                          G_PARAM_READWRITE );
   validated_entry_properties[PROP_VALID] = g_param_spec_boolean("valid",
                                                                     "valid",
                                                                     "valid",
                                                                     TRUE,
                                                                     G_PARAM_READWRITE );
   validated_entry_properties[PROP_DEFAULT_TEXT_FALLBACK] = g_param_spec_string("default_text_fallback",
                                                             "default_text_fallback",
                                                             "default_text_fallback",
                                                             NULL,
                                                             G_PARAM_READWRITE );

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/validated_entry");
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, title_label);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, message_label);

   // This could be a component/composite interface
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, entry_label);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, entry_to_validate);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, chckbtn_modify_override);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_chckbtn_modify_override_toggled", (GCallback)on_chckbtn_modify_override_toggled);

   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, btn_validate_and_submit);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, btn_cancel);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_validate_and_submit_clicked", (GCallback)on_btn_validate_and_submit_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_cancel_clicked", (GCallback)on_btn_cancel_clicked);
}

static void validated_entry_init(ValidatedEntry *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *validated_entry_new(const gchar *title, const gchar *msg,
                               const gchar *entry_label_text,
                               const gchar *default_entry_text,
                               const gchar *modify_override_text,
                                const gchar *btn_validate_and_submit_text, const gchar *btn_cancel_text,
                               ValidateFormCallback_T return_callback)
{
   ValidatedEntry *self;
   self = g_object_new(VALIDATED_TYPE_ENTRY, NULL);

   gtk_label_set_label(GTK_LABEL(self->title_label), title);
   gtk_label_set_label(GTK_LABEL(self->message_label), msg);

   gtk_label_set_label(GTK_LABEL(self->entry_label), entry_label_text);
   gtk_entry_set_text(GTK_ENTRY(self->entry_to_validate), default_entry_text);

   if (default_entry_text != NULL)
   {
      validated_entry_set_default_text_fallback(self, default_entry_text);
   }

   gtk_button_set_label(GTK_BUTTON(self->chckbtn_modify_override), modify_override_text);

   gtk_button_set_label(GTK_BUTTON(self->btn_validate_and_submit), btn_validate_and_submit_text);
   gtk_button_set_label(GTK_BUTTON(self->btn_cancel), btn_cancel_text);

   if (return_callback != NULL)
   {
      self->validate_and_submit_callback = return_callback;
   }
   return GTK_WIDGET(self);
}

static void validated_entry_finalize( GObject *oSelf )
{
   g_return_if_fail(oSelf != NULL);
   g_return_if_fail(VALIDATED_IS_ENTRY(oSelf));
   ValidatedEntry *self = VALIDATED_ENTRY(oSelf);
   g_free(self->default_text_fallback);

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (validated_entry_parent_class)->finalize (oSelf);
}

gboolean validated_entry_get_sensitive( ValidatedEntry *self )
{
   g_return_val_if_fail( VALIDATED_IS_ENTRY( self ), FALSE );
   return self->sensitive;
}
void validated_entry_set_sensitive( ValidatedEntry *self, gboolean sensitive )
{
   g_return_if_fail( VALIDATED_IS_ENTRY( self ) );
   self->sensitive = sensitive;
   g_object_notify_by_pspec(G_OBJECT(self), validated_entry_properties[PROP_SENSITIVE]);
}

gboolean validated_entry_get_valid( ValidatedEntry *self)
{
   g_return_val_if_fail( VALIDATED_IS_ENTRY( self ), FALSE );
   return self->valid;
}
void validated_entry_set_valid( ValidatedEntry *self, gboolean valid)
{
   g_return_if_fail( VALIDATED_IS_ENTRY( self ) );
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
   // g_object_notify_by_pspec(G_OBJECT(self), validated_entry_properties[PROP_VALID]);
}

gchar *validated_entry_get_default_text_fallback( ValidatedEntry *self )
{
   g_return_val_if_fail( VALIDATED_IS_ENTRY( self ), NULL );

   return g_strdup( self->default_text_fallback );
}
void validated_entry_set_default_text_fallback( ValidatedEntry *self, const gchar *default_text )
{
   g_return_if_fail( default_text );
   g_return_if_fail( VALIDATED_IS_ENTRY( self ) );

   if( self->default_text_fallback != NULL )
      g_free( self->default_text_fallback );

   self->default_text_fallback = g_strdup( default_text );
}