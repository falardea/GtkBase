/**
 * @file setup_context_loader.h
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
#ifndef SETUP_CONTEXT_LOADER_H__
#define SETUP_CONTEXT_LOADER_H__
#include <gtk/gtk.h>

#include "views/setup_wizard/models/run_model.h"

G_BEGIN_DECLS

#define SETUP_TYPE_CONTEXT_LOADER (setup_context_loader_get_type())

G_DECLARE_FINAL_TYPE(SetupContextLoader, setup_context_loader, SETUP, CONTEXT_LOADER, GtkBox)

SetupContextLoader *setup_context_loader_new();

void setup_context_loader_set_context(SetupContextLoader *self, RunModel *model);
void setup_context_loader_exec_strategy(SetupContextLoader *self);

RUN_MODEL_PHASES setup_context_loader_get_run_phase( SetupContextLoader *self );
void setup_context_loader_set_run_phase( SetupContextLoader *self, RUN_MODEL_PHASES phase );


// void setup_context_loader_set_clinical_strategy(SetupContextLoader *self, RunModel *model);
// void setup_context_loader_set_maintenance_strategy(SetupContextLoader *self, RunModel *model);
// void setup_context_loader_set_calibration_strategy(SetupContextLoader *self, RunModel *model);

G_END_DECLS
#endif  // SETUP_CONTEXT_LOADER_H__
