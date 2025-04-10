/**
 * @file step_sequence.c
 *
 * @copyright
 * COPYRIGHT 2025
 * DEKA RESEARCH AND DEVELOPMENT CORPORATION
 *
 * Contains confidential and proprietary information which
 * may not be copied, disclosed or used by others except as expressly
 * authorized in writing by DEKA Research & Development Corporation.
 *
 * @critical <Minor, Moderate, Major Single-Point, Major Multi-Point>
 * @brief <brief description of the file>
 * @subsystem <subsystem name>
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