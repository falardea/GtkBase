/**
 * @file timeout_setupstep.h
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

#ifndef TIMEOUT_SETUP_STEP_H__
#define TIMEOUT_SETUP_STEP_H__
#include <gtk/gtk.h>
#include "app_globals.h"

G_BEGIN_DECLS

#define TIMEOUT_TYPE_SETUP_STEP (timeout_setup_step_get_type())

G_DECLARE_FINAL_TYPE(TimeoutSetupStep, timeout_setup_step, TIMEOUT, SETUP_STEP, GtkBox)

typedef void (*OnTimoutExpiredCallback_T)(gpointer user_data);

GtkWidget* timeout_setup_step_new(const gchar *step_description,
                                  guint countdown,
                                  OnTimoutExpiredCallback_T on_timeout,
                                  gpointer callback_user_data);

void timeout_setup_step_execute(TimeoutSetupStep *self);

G_END_DECLS
#endif  // TIMEOUT_SETUP_STEP_H__
