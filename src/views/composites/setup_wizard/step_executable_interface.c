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

void step_executable_execute(StepExecutable *self)
{
   StepExecutableInterface *iface;
   g_return_if_fail(STEP_IS_EXECUTABLE(self));

   iface = STEP_EXECUTABLE_GET_IFACE(self);

   iface->execute(self);
}