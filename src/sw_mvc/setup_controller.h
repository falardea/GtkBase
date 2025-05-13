/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef SETUP_CONTROLLER_H__
#define SETUP_CONTROLLER_H__
#include <gtk/gtk.h>
#include "run_model.h"
#include "run_viewer.h"
#include "setup_view.h"
G_BEGIN_DECLS // __cplusplus guard

#define SETUP_TYPE_CONTROLLER (setup_controller_get_type())

G_DECLARE_FINAL_TYPE(SetupController, setup_controller, SETUP, CONTROLLER, GObject)

SetupController *setup_controller_new(RunViewer *run_viewer, SetupViewer *setup_viewer, RunModel *run_model);

void setup_controller_build_setup_viewer(SetupController *self);

G_END_DECLS
#endif  // SETUP_CONTROLLER_H__
