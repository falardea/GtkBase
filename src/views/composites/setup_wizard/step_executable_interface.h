/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_EXECUTABLE_INTERFACE_H__
#define STEP_EXECUTABLE_INTERFACE_H__
#include <glib-object.h>

#define STEP_TYPE_EXECUTABLE (step_executable_get_type())

G_DECLARE_INTERFACE(StepExecutable, step_executable, STEP, EXECUTABLE, GObject)

typedef struct _StepExecutableInterface
{
   GTypeInstance parent;
   void (*execute) (StepExecutable *self);
}StepExecutableInterface;

void step_executable_execute(StepExecutable *self);

#endif  // STEP_EXECUTABLE_INTERFACE_H__
