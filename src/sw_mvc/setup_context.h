/**
 * Created by french on 4/29/25.
 * @brief
 */
#ifndef SETUP_CONTEXT_H__
#define SETUP_CONTEXT_H__
#include <gtk/gtk.h>
#include "sw_mvc/run_model.h"
G_BEGIN_DECLS // __cplusplus guard

#define SETUP_TYPE_CONTEXT (setup_context_get_type())

G_DECLARE_FINAL_TYPE(SetupContext, setup_context, SETUP, CONTEXT, GtkBox)

SetupContext *setup_context_new(RunModel *model);

G_END_DECLS
#endif  // SETUP_CONTEXT_H__
