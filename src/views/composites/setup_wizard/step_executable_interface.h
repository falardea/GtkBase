/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_EXECUTABLE_INTERFACE_H__
#define STEP_EXECUTABLE_INTERFACE_H__
#include <glib-object.h>

G_BEGIN_DECLS

#define BLUE_FORMAT_STR(x)               "<span foreground='#0228b1' size='xx-large'>" x "</span>"
#define BLUE_BULLET_FORMAT_STR           BLUE_FORMAT_STR("\u2022")
#define BLUE_SELECTED_BULLET_FORMAT_STR  BLUE_FORMAT_STR("\u25ba")

#define STEP_TYPE_EXECUTABLE     (step_executable_get_type())

G_DECLARE_INTERFACE (StepExecutable, step_executable, STEP, EXECUTABLE, GObject)

typedef void (*ExecutableCallback_T)(StepExecutable *parent_sequence, gpointer user_data);

struct _StepExecutableInterface
{
   GTypeInterface g_iface;
   void (*execute) (StepExecutable *self, gpointer user_data);

   gpointer padding[10];
};

void step_executable_execute(StepExecutable *self, gpointer user_data);

G_END_DECLS
#endif  // STEP_EXECUTABLE_INTERFACE_H__
