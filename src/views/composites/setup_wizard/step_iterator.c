/**
 * Created by french on 4/26/25.
 * @brief
 */
#include "step_iterator.h"

struct _StepIterator
{
   GObject  parent_object;
   GList    *steps;
   GList    *current;
};

G_DEFINE_TYPE(StepIterator, step_iterator, G_TYPE_OBJECT)

static void step_iterator_finalize(GObject *g_object)
{
   G_OBJECT_CLASS(step_iterator_parent_class)->finalize (g_object);
}

static void step_iterator_class_init(StepIteratorClass *klass)
{
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);
   g_object_class->finalize = step_iterator_finalize;
}

static void step_iterator_init(StepIterator *iter)
{
   iter->current  = NULL;
   iter->steps    = NULL;
}

StepIterator *step_iterator_new(GList *steps)
{
   StepIterator *iter;

   iter = g_object_new(STEP_TYPE_ITERATOR, NULL);

   iter->steps = steps;
   iter->current = steps;
   return iter;
}

StepExecutable *step_iterator_next(StepIterator *iter)
{
   g_return_val_if_fail(STEP_IS_ITERATOR(iter), NULL);
   if(iter->current)
   {
      StepExecutable *step = STEP_EXECUTABLE(iter->current->data);
      iter->current = g_list_next(iter->current);
      return step;
   }
   return NULL;
}

gboolean step_iterator_has_next(StepIterator *iter)
{
//   g_return_val_if_fail(STEP_IS_ITERATOR(iter), FALSE);
   return (iter != NULL) ? (iter->current != NULL) : FALSE;
}
