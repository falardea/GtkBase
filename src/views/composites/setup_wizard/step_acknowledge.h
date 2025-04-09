/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_ACKNOWLEDGE_H__
#define STEP_ACKNOWLEDGE_H__
#include <gtk/gtk.h>
#include "app_globals.h"

G_BEGIN_DECLS

#define STEP_TYPE_ACKNOWLEDGE (step_acknowledge_get_type())

G_DECLARE_FINAL_TYPE(StepAcknowledge, step_acknowledge, STEP, ACKNOWLEDGE, GtkBox)

StepAcknowledge *step_acknowledge_new(const gchar *step_description,
                                      const gchar *btn_label_str,
                                      SequenceRunner *parent_sequence,
                                      SequenceCallback_T on_acknowledge,
                                      gpointer callback_user_data);


G_END_DECLS
#endif  // STEP_ACKNOWLEDGE_H__
