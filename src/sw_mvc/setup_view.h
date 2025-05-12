#ifndef SETUP_VIEW_H__
#define SETUP_VIEW_H__
#include <gtk/gtk.h>
#include "run_model.h"
G_BEGIN_DECLS

#define SETUP_TYPE_VIEWER (setup_viewer_get_type())

G_DECLARE_FINAL_TYPE(SetupViewer, setup_viewer, SETUP, VIEWER, GtkBox)

SetupViewer *setup_viewer_new(RunModel *model);

G_END_DECLS
#endif  // SETUP_VIEW_H__
