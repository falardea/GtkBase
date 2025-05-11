/**
 * Created by french on 5/10/25.
 * @brief
 */
#include "app_model_context_manager_builder.h"
#include "context_mediator.h"
#include "standard_context.h"
#include "service_context.h"
#include "setup_context.h"
#include "utils/logging.h"

ContextMediator *build_context_for_run_mode(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   ContextMediator *ret_context;
   if (run_model_get_run_mode(model) == RUN_MODE_STANDARD)
   {
      if(run_model_get_last_completed_phase(model) == RUN_SETUP_COMPLETE)
      {
         // if setup complete, load the re-calibration context
         ret_context = CONTEXT_MEDIATOR(standard_context_new(model));
      }
      else
      {
         ret_context = CONTEXT_MEDIATOR(setup_context_new(model));
      }
   }
   else
   {
      ret_context = CONTEXT_MEDIATOR(service_context_new(model));
   }
   return ret_context;
}
