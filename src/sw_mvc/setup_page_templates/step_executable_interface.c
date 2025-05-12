/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_executable_interface.h"
#include "utils/logging.h"

G_DEFINE_INTERFACE(StepExecutable, step_executable, G_TYPE_OBJECT)

static void step_executable_default_init(__attribute__((unused)) StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

void step_executable_execute(StepExecutable *self, RunModel *model)
{
   g_return_if_fail(STEP_IS_EXECUTABLE(self));
   StepExecutableInterface *iface;
   iface = STEP_EXECUTABLE_GET_IFACE(self);
   iface->execute(self, model);
}

StepExecutable *step_executable_get_next(StepExecutable *self, RunModel *model)
{
   g_return_val_if_fail(STEP_IS_EXECUTABLE(self), NULL);
   StepExecutableInterface *iface;
   iface = STEP_EXECUTABLE_GET_IFACE(self);
   return iface->get_next(self, model);
}