/**
 * Created by french on 4/26/25.
 * @brief
 */
#ifndef STEP_ITERATOR_H__
#define STEP_ITERATOR_H__
#include <gtk/gtk.h>
#include "step_executable_interface.h"

G_BEGIN_DECLS

typedef struct _StepExecutable StepExecutable;

#define STEP_TYPE_ITERATOR (step_iterator_get_type())

G_DECLARE_FINAL_TYPE(StepIterator, step_iterator, STEP, ITERATOR, GObject)

StepIterator *step_iterator_new(GList *steps);  // GPtrArray?
StepExecutable *step_iterator_next(StepIterator *iter);
gboolean step_iterator_has_next(StepIterator *iter);

G_END_DECLS
#endif  // STEP_ITERATOR_H__
