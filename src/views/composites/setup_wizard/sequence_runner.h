/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef SETUP_RUNNER_H__
#define SETUP_RUNNER_H__
#include <gtk/gtk.h>

G_BEGIN_DECLS

void execute_sample_timeout_step(gpointer user_data);

void on_sample_timeout_step_complete(gpointer user_data);
void on_secondary_timeout_step_complete(gpointer user_data);

G_END_DECLS
#endif  // SETUP_RUNNER_H__
