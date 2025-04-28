/**
 * Created by french on 3/22/25.
 * @brief
 */

#ifndef STEP_EXECUTABLE_INTERFACE_H__
#define STEP_EXECUTABLE_INTERFACE_H__
#include <glib-object.h>
#include "run_model.h"
#include "step_iterator.h"

G_BEGIN_DECLS

#define BLUE_FORMAT_STR(x)               "<span foreground='#0228b1' size='xx-large'>" x "</span>"
#define BLUE_BULLET_FORMAT_STR           BLUE_FORMAT_STR("\u2022")
#define BLUE_SELECTED_BULLET_FORMAT_STR  BLUE_FORMAT_STR("\u25ba")

#define STEP_TYPE_EXECUTABLE     (step_executable_get_type())

typedef struct _StepIterator StepIterator;

G_DECLARE_INTERFACE (StepExecutable, step_executable, STEP, EXECUTABLE, GObject)

typedef void (*ExecutableCallback_T)(StepExecutable *parent_sequence, RunModel *run_model);
typedef gboolean (*ExecutableConditionCallback_T)(StepExecutable *parent_sequence, RunModel *run_model);

struct _StepExecutableInterface
{
   GTypeInterface g_iface;
   void (*execute) (StepExecutable *self, RunModel *run_model);
   void (*cancel) (StepExecutable *self, RunModel *run_model);
   StepIterator *(*get_iterator)(StepExecutable *self);
   gpointer padding[10];
};

void step_executable_execute(StepExecutable *self, RunModel *run_model);
void step_executable_cancel(StepExecutable *self, RunModel *run_model);
StepIterator *step_executable_get_iterator(StepExecutable *self, RunModel *run_model);

G_END_DECLS
#endif  // STEP_EXECUTABLE_INTERFACE_H__
