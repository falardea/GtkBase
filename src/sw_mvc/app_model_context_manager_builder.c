/**
 * Created by french on 5/10/25.
 * @brief
 */
#include "app_model_context_manager_builder.h"
#include "standard_context.h"
#include "service_context.h"
#include "setup_context.h"
#include "utils/logging.h"

GtkBox *build_context_for_run_mode(RunModel *model)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   GtkBox *ret_context;
   if (run_model_get_run_mode(model) == RUN_MODE_STANDARD)
   {
      if(run_model_get_last_completed_phase(model) == RUN_SETUP_COMPLETE)
      {
         // if setup complete, load the re-calibration context
         ret_context = GTK_BOX(standard_context_new(model));
      }
      else
      {
         ret_context = GTK_BOX(setup_context_new(model));
      }
   }
   else
   {
      ret_context = GTK_BOX(service_context_new(model));
   }
   return ret_context;
}
