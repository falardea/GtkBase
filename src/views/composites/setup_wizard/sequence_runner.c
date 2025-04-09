/**
 * Created by french on 3/22/25.
 * @brief
 */
#include "app_globals.h"
#include "sequence_runner.h"
#include "timeout_setup_step.h"



void execute_sample_timeout_step(gpointer user_data)
{
   timeout_setup_step_execute(TIMEOUT_SETUP_STEP(g_app_widget_refs->w_sample_timeout_step));
}

void on_sample_timeout_step_complete(gpointer user_data)
{
   g_print("%s: on to step 2\n", __func__);
   timeout_setup_step_execute(TIMEOUT_SETUP_STEP(g_app_widget_refs->w_secondary_timeout_step));
}

void on_secondary_timeout_step_complete(gpointer user_data)
{
   g_print("%s\n", __func__);
}

