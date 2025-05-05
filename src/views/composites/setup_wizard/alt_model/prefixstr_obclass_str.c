/**
 * Created by french on 4/29/25.
 * @brief
 */
#include "prefixstr_obclass_str.h"

typedef struct
{
   // There isn't much need for "private" data you are going to keep a reference
   // count to something that doesn't get ref-counted elsewhere.
   // I'm not sure if private data can be configured as a property on the object
   gboolean    private_flag;
} PrefixstrObclassStrPrivate;

struct _PrefixstrObclassStr
{
   // This is the class definitional object
   GObject  super;

   // This is the "public" data associated with our GObject, variables defined here
   // can be configured as properties on the object that can be used to bind to other
   // object properties or to notify objects bound to these properties.
   gboolean public_flag;
};

enum {
   PROP_O,
   OBCLASS_STR_PUBLIC_FLAG,
   N_OBCLASS_STR_PROPERTIES
};

G_DEFINE_TYPE_WITH_PRIVATE(PrefixstrObclassStr, prefixstr_obclass_str, G_TYPE_OBJECT)

static void prefixstr_obclass_str_finalize(GObject *g_object)
{
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(PREFIXSTR_IS_OBCLASS_STR(g_object));
//   PrefixstrObclassStr *self = PREFIXSTR_OBCLASS_STR(g_object);

   G_OBJECT_CLASS(prefixstr_obclass_str_parent_class)->finalize(g_object);
}

static void prefixstr_obclass_str_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
   PrefixstrObclassStr *self = PREFIXSTR_OBCLASS_STR(object);
   switch(prop_id){
      case OBCLASS_STR_PUBLIC_FLAG:
         prefixstr_obclass_str_set_public_flag(self, g_value_get_boolean( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void prefixstr_obclass_str_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
   PrefixstrObclassStr *self = PREFIXSTR_OBCLASS_STR(object);
   switch(prop_id)
   {
      case OBCLASS_STR_PUBLIC_FLAG:
         g_value_set_boolean( value, prefixstr_obclass_str_get_public_flag(self));
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static GParamSpec *obclass_str_properties[N_OBCLASS_STR_PROPERTIES] = {NULL, };

static void prefixstr_obclass_str_class_init(PrefixstrObclassStrClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   gobject_class->finalize = prefixstr_obclass_str_finalize;

   gobject_class->set_property = prefixstr_obclass_str_set_property;
   gobject_class->get_property = prefixstr_obclass_str_get_property;

   obclass_str_properties[OBCLASS_STR_PUBLIC_FLAG] = g_param_spec_boolean("public-flag",
                                                                          "Public Flag Enabled",
                                                                          "Obclass_str public_flag enabled state",
                                                                          FALSE,
                                                                          G_PARAM_READWRITE );
   g_object_class_install_properties( gobject_class, N_OBCLASS_STR_PROPERTIES, obclass_str_properties);
}

static void prefixstr_obclass_str_init(PrefixstrObclassStr *self)
{
   g_return_if_fail(PREFIXSTR_IS_OBCLASS_STR(self));
   PrefixstrObclassStrPrivate *priv = prefixstr_obclass_str_get_instance_private(self);
   priv->private_flag = FALSE;
}

PrefixstrObclassStr *prefixstr_obclass_str_new()
{
   PrefixstrObclassStr *self;
   self = g_object_new(PREFIXSTR_TYPE_OBCLASS_STR, NULL);

   return self;
}

void prefixstr_obclass_str_set_public_flag(PrefixstrObclassStr *self, gboolean enable)
{
   g_return_if_fail(PREFIXSTR_IS_OBCLASS_STR(self));
   self->public_flag = enable;
   g_object_notify_by_pspec(G_OBJECT(self), obclass_str_properties[OBCLASS_STR_PUBLIC_FLAG]);
}

gboolean prefixstr_obclass_str_get_public_flag(PrefixstrObclassStr *self)
{
   g_return_val_if_fail(PREFIXSTR_IS_OBCLASS_STR(self), FALSE);
   return self->public_flag;
}
