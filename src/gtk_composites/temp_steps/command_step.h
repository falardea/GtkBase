/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef COMMAND_STEP_H__
#define COMMAND_STEP_H__
#include <gtk/gtk.h>
#include "app_widgets.h"
#include "views/setup_wizard/models/run_model.h"
G_BEGIN_DECLS

#define COMMAND_TYPE_STEP (command_step_get_type())

G_DECLARE_FINAL_TYPE(CommandStep, command_step, COMMAND, STEP, GtkBox)

CommandStep *command_step_new(const gchar *command_description, RunModel *model);

G_END_DECLS
#endif  // COMMAND_STEP_H__
