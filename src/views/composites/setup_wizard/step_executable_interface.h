/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_EXECUTABLE_INTERFACE_H__
#define STEP_EXECUTABLE_INTERFACE_H__
#include <glib-object.h>

G_BEGIN_DECLS

#define STEP_TYPE_EXECUTABLE     (step_executable_get_type())

G_DECLARE_INTERFACE (StepExecutable, step_executable, STEP, EXECUTABLE, GObject)

struct _StepExecutableInterface
{
   GTypeInterface g_iface;
   void (*execute) (StepExecutable *self);

   gpointer padding[10];
};

void step_executable_execute(StepExecutable *self);

G_END_DECLS
#endif  // STEP_EXECUTABLE_INTERFACE_H__
