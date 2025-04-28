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

void step_executable_execute(StepExecutable *self, RunModel *run_model)
{
   StepExecutableInterface *iface;
   g_return_if_fail(STEP_IS_EXECUTABLE(self));
   iface = STEP_EXECUTABLE_GET_IFACE(self);
   g_return_if_fail(iface->execute != NULL);
   iface->execute(self, run_model);
}

void step_executable_cancel(StepExecutable *self, RunModel *run_model)
{
   StepExecutableInterface *iface;
   g_return_if_fail(STEP_IS_EXECUTABLE(self));
   iface = STEP_EXECUTABLE_GET_IFACE(self);
   if (iface->cancel)
   {
      iface->cancel(self, run_model);
   }
}

StepIterator *step_executable_get_iterator(StepExecutable *self, RunModel *run_model)
{
   StepExecutableInterface *iface;
   g_return_val_if_fail(STEP_IS_EXECUTABLE(self), NULL);
   iface = STEP_EXECUTABLE_GET_IFACE(self);
   if(iface->get_iterator)
   {
      return iface->get_iterator(self);
   }
   return NULL;
}
