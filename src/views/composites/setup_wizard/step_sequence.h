/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_SEQUENCE_H__
#define STEP_SEQUENCE_H__
#include <gtk/gtk.h>
#include "app_globals.h"
#include "sequence_runner.h"
G_BEGIN_DECLS

#define STEP_TYPE_SEQUENCE (step_sequence_get_type())

G_DECLARE_FINAL_TYPE(StepSequence, step_sequence, STEP, SEQUENCE, GtkBox)

StepSequence *step_sequence_new(const gchar *sequence_description,
                                const gchar *btn_label_str,
                                SequenceRunner *parent_sequence,
                                SequenceCallback_T on_sequence_complete,
                                gpointer callback_user_data);

G_END_DECLS
#endif  // STEP_SEQUENCE_H__
