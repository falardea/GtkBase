/**
 * Created by french on 10/5/24.
 * @brief
 */
#ifndef MODE_PROMPT_H__
#define MODE_PROMPT_H__
#include "app_widgets.h"
G_BEGIN_DECLS

#define MODE_TYPE_PROMPT             ( mode_prompt_get_type () )
G_DECLARE_FINAL_TYPE(ModePrompt, mode_prompt, MODE, PROMPT, GtkBox )

typedef gboolean (*ValidateFormCallback_T)(const gchar *text_to_validate);

GtkWidget*        mode_prompt_new       (ValidateFormCallback_T return_callback);

gboolean mode_prompt_get_sensitive( ModePrompt *self );
void mode_prompt_set_sensitive( ModePrompt *self, gboolean sensitive );

gboolean mode_prompt_get_valid( ModePrompt *self);
void mode_prompt_set_valid( ModePrompt *self, gboolean valid);

gchar *mode_prompt_get_default_text_fallback( ModePrompt *self );
void mode_prompt_set_default_text_fallback( ModePrompt *self, const gchar *name );

G_END_DECLS
#endif  // MODE_PROMPT_H__
