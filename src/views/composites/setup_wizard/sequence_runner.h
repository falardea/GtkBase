/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SEQUENCE_RUNNER_H__
#define SEQUENCE_RUNNER_H__
#include <gtk/gtk.h>
#include "step_executable_interface.h"

G_BEGIN_DECLS

#define SEQUENCE_TYPE_RUNNER (sequence_runner_get_type())

G_DECLARE_FINAL_TYPE(SequenceRunner, sequence_runner, SEQUENCE, RUNNER, GtkBox)

SequenceRunner *sequence_runner_new(ExecutableCallback_T on_sequence_complete,
                                    gpointer callback_user_data);

void sequence_runner_add_child(SequenceRunner *self, StepExecutable *child);

G_END_DECLS
#endif  // SEQUENCE_RUNNER_H__
