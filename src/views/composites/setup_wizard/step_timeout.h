/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_TIMEOUT_H__
#define STEP_TIMEOUT_H__
#include <gtk/gtk.h>
#include "app_globals.h"
#include "sequence_runner.h"
#include "step_executable_interface.h"

G_BEGIN_DECLS

#define STEP_TYPE_TIMEOUT (step_timeout_get_type())

G_DECLARE_FINAL_TYPE(StepTimeout, step_timeout, STEP, TIMEOUT, GtkBox)

StepTimeout* step_timeout_new(const gchar *step_description,
                              guint countdown,
                              SequenceRunner *parent_sequence,
                              SequenceCallback_T on_timeout,
                              gpointer callback_user_data);

G_END_DECLS
#endif  // STEP_TIMEOUT_H__
