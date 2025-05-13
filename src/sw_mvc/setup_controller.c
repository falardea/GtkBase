/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "setup_controller.h"
#include "utils/logging.h"

struct _SetupController
{
   GObject super;

   SetupViewer *setup_viewer;
   RunModel    *run_model;

   RunViewer   *run_viewer;
};

G_DEFINE_TYPE(SetupController, setup_controller, G_TYPE_OBJECT)

static void setup_controller_finalize(GObject *g_object)
{
   G_OBJECT_CLASS(setup_controller_parent_class)->finalize(g_object);
}

static void setup_controller_class_init(SetupControllerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = setup_controller_finalize;
}

static void setup_controller_init(SetupController *self)
{
}

SetupController *setup_controller_new(RunViewer *run_viewer, SetupViewer *setup_viewer, RunModel *run_model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   SetupController *self;
   self = g_object_new(SETUP_TYPE_CONTROLLER, NULL);

   self->run_viewer = run_viewer;
   self->setup_viewer = setup_viewer;
   self->run_model = run_model;

   return self;
}
