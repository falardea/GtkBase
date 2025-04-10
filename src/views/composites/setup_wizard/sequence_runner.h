/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SEQUENCE_RUNNER_H__
#define SEQUENCE_RUNNER_H__
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SEQUENCE_TYPE_RUNNER (sequence_runner_get_type())

G_DECLARE_FINAL_TYPE(SequenceRunner, sequence_runner, SEQUENCE, RUNNER, GtkBox)

typedef void (*SequenceCallback_T)(SequenceRunner *parent_sequence, gpointer user_data);

SequenceRunner *sequence_runner_new();

G_END_DECLS
#endif  // SEQUENCE_RUNNER_H__
