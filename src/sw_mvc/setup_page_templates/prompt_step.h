/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef PROMPT_STEP_H__
#define PROMPT_STEP_H__
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define PROMPT_TYPE_STEP (prompt_step_get_type())

G_DECLARE_FINAL_TYPE(PromptStep, prompt_step, PROMPT, STEP, GtkBox)

PromptStep *prompt_step_new(const gchar *prompt_description);

G_END_DECLS
#endif  // PROMPT_STEP_H__
