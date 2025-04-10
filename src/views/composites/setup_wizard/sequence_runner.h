/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SEQUENCE_RUNNER_H__
#define SEQUENCE_RUNNER_H__
#include <gtk/gtk.h>

G_BEGIN_DECLS

//#define BLUE_BULLET_FORMAT_STR           BLUE_FORMAT_STR(" \u2022")  // Note the intentional space for alignment

#define BLUE_FORMAT_STR(x)               "<span foreground='#0228b1' size='xx-large'>" x "</span>"
#define BLUE_BULLET_FORMAT_STR           BLUE_FORMAT_STR("\u2022")
#define BLUE_SELECTED_BULLET_FORMAT_STR  BLUE_FORMAT_STR("\u25ba")

#define SEQUENCE_TYPE_RUNNER (sequence_runner_get_type())

G_DECLARE_FINAL_TYPE(SequenceRunner, sequence_runner, SEQUENCE, RUNNER, GtkBox)

typedef void (*SequenceCallback_T)(SequenceRunner *parent_sequence, gpointer user_data);

SequenceRunner *sequence_runner_new();

G_END_DECLS
#endif  // SEQUENCE_RUNNER_H__
