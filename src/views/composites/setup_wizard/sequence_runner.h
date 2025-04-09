/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SETUP_RUNNER_H__
#define SETUP_RUNNER_H__
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SEQUENCE_TYPE_RUNNER (sequence_runner_get_type())

G_DECLARE_FINAL_TYPE(SequenceRunner, sequence_runner, SEQUENCE, RUNNER, GtkBox)

SequenceRunner *sequence_runner_new();

void sequence_runner_execute();

void sequence_runner_validate_sequence();

G_END_DECLS
#endif  // SETUP_RUNNER_H__
