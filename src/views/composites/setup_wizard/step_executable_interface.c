/**
 * Created by french on 3/22/25.
 * @brief
 */

#include "step_executable_interface.h"
#include "utils/logging.h"

typedef StepExecutableInterface StepExecutableInterface;

G_DEFINE_INTERFACE(StepExecutable, step_executable, G_TYPE_OBJECT)

static void step_executable_default_init(StepExecutableInterface *iface)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
}

void step_executable_execute(StepExecutable *self)
{
   STEP_EXECUTABLE_GET_IFACE(self)->execute(self);
}