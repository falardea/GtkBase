/**
 * @file setup_runner.h
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

#ifndef SETUP_RUNNER_H__
#define SETUP_RUNNER_H__
#include <gtk/gtk.h>

void execute_sample_timeout_step(gpointer user_data);

void on_sample_timeout_step_complete(gpointer user_data);
void on_secondary_timeout_step_complete(gpointer user_data);

#endif  // SETUP_RUNNER_H__
