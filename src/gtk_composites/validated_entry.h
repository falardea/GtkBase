/**
 * Created by french on 10/5/24.
 * @brief
 */
#ifndef VALIDATED_ENTRY_H__
#define VALIDATED_ENTRY_H__
#include "app_globals.h"
G_BEGIN_DECLS

#define VALIDATED_TYPE_ENTRY             ( validated_entry_get_type () )
G_DECLARE_FINAL_TYPE(ValidatedEntry, validated_entry, VALIDATED, ENTRY, GtkBox )

typedef gboolean (*ValidateFormCallback_T)(const gchar *text_to_validate);

GtkWidget*        validated_entry_new       (const gchar *title, const gchar *msg,
                                             const gchar *entry_label_text,
                                             const gchar *default_entry_text,
                                             const gchar *modify_override_text,
                                             const gchar *btn_validate_and_submit_text, const gchar *btn_cancel_text,
                                             ValidateFormCallback_T return_callback);

gboolean validated_entry_get_sensitive( ValidatedEntry *self );
void validated_entry_set_sensitive( ValidatedEntry *self, gboolean sensitive );

gboolean validated_entry_get_valid( ValidatedEntry *self);
void validated_entry_set_valid( ValidatedEntry *self, gboolean valid);

gchar *validated_entry_get_default_text_fallback( ValidatedEntry *self );
void validated_entry_set_default_text_fallback( ValidatedEntry *self, const gchar *name );

G_END_DECLS
#endif  // VALIDATED_ENTRY_H__
