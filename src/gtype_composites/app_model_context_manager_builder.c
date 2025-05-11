/**
 * Created by french on 5/10/25.
 * @brief
 */
#include "app_model_context_manager_builder.h"

#include "gtk_composites/standard_context.h"
#include "gtk_composites/service_context.h"

GtkBox *build_context_for_app_mode(AppModel *model)
{
   if (app_model_get_run_mode(model) == RUN_MODE_STANDARD)
   {
      // if setup complete, load the re-calibration context
      return GTK_BOX(standard_context_new(model));
   }
   else
   {
      return GTK_BOX(service_context_new(model));
   }
}
