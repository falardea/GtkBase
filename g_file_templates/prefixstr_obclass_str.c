/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "prefixstr_obclass_str.h"

struct _PrefixstrObclassStr
{
   GObject                       super;
   GtkWidget                     *parent_context;
};

G_DEFINE_TYPE(PrefixstrObclassStr, prefixstr_obclass_str, G_TYPE_OBJECT)

static void prefixstr_obclass_str_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PREFIXSTR_IS_OBCLASS_STR(g_object));
   PrefixstrObclassStr *self = PREFIXSTR_OBCLASS_STR(g_object);
   G_OBJECT_CLASS(prefixstr_obclass_str_parent_class)->finalize(g_object);
}

static void prefixstr_obclass_str_class_init(PrefixstrObclassStrClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = prefixstr_obclass_str_finalize;
}

static void prefixstr_obclass_str_init(PrefixstrObclassStr *self)
{
   self->parent_context = NULL;
}

PrefixstrObclassStr *prefixstr_obclass_str_new(GtkWidget *parent_box)
{
   PrefixstrObclassStr *self;
   self = g_object_new(PREFIXSTR_TYPE_OBCLASS_STR, NULL);

   self->parent_context = parent_box;

   return self;
}
