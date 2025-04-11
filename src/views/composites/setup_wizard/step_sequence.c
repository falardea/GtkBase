/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_sequence.h"
#include "step_executable_interface.h"
#include "utils/logging.h"

struct _StepSequence
{
   GtkBox      parent;
   GtkLabel    *lbl_step_bullet;
   GtkLabel    *lbl_step_description;
   GtkButton   *btn_start_sequence;

   void (*on_sequence_complete)(SequenceRunner *parent_sequence, gpointer user_data);
   gpointer callback_user_data;

   SequenceRunner *parent_sequence;
};