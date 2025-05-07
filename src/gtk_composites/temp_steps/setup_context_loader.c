/**
 * @file setup_context_loader.c
 *
 * @copyright
 * COPYRIGHT 2025
 * DEKA RESEARCH AND DEVELOPMENT CORPORATION
 *
 * Contains confidential and proprietary information which
 * may not be copied, disclosed or used by others except as expressly
 * authorized in writing by DEKA Research & Development Corporation.
 *
 * @critical <Minor, Moderate, Major Single-Point, Major Multi-Point>
 * @brief <brief description of the file>
 * @subsystem <subsystem name>
 */

#include "setup_context_loader.h"
#include "utils/logging.h"

#include "sw_v2/command_step.h"
#include "sw_v2/pending_step.h"

typedef struct
{
   RunModel   *model;
} SetupContextLoaderPrivate;

struct _SetupContextLoader
{
   GtkBox            parent;
   GtkBox            *context_box;

   RUN_MODEL_PHASES  current_phase;
};

G_DEFINE_TYPE_WITH_PRIVATE(SetupContextLoader, setup_context_loader, GTK_TYPE_BOX)

enum
{
   SCL_PROP_0 = 0, // Reserved for GObject
   SCL_PROP_CURRENT_RUN_PHASE,
   SCL_N_PROPERTIES
};

static void setup_context_loader_finalize(GObject *g_object)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   g_return_if_fail(g_object != NULL);
   g_return_if_fail(SETUP_IS_CONTEXT_LOADER(g_object));

   G_OBJECT_CLASS(setup_context_loader_parent_class)->finalize(g_object);
}

static void setup_context_loader_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
   SetupContextLoader *self = RUN_MODEL( object );

   switch( prop_id ) {
      case SCL_PROP_CURRENT_RUN_PHASE:
         setup_context_loader_set_run_phase( self, g_value_get_uint( value ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void setup_context_loader_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
   SetupContextLoader *self = RUN_MODEL( object );

   switch( prop_id ) {
      case SCL_PROP_CURRENT_RUN_PHASE:
         g_value_set_uint( value, setup_context_loader_get_run_phase( self ) );
         break;
      default:
         G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
   }
}

static void setup_context_loader_class_init(SetupContextLoaderClass *klass)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

   gobject_class->finalize = setup_context_loader_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(widget_class), "/com/dekaresearch/pod/setup_context_loader");
   gtk_widget_class_bind_template_child_internal(widget_class, SetupContextLoader, context_box);
}

static void setup_context_loader_init(SetupContextLoader *self)
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   SetupContextLoaderPrivate *priv = setup_context_loader_get_instance_private(self);

   gtk_widget_init_template(GTK_WIDGET(self));
   priv->model = NULL;
}

SetupContextLoader *setup_context_loader_new()
{
   logging_llprintf(LOGLEVEL_TRACE, "%s", __func__);
   SetupContextLoader *self = g_object_new(SETUP_TYPE_CONTEXT_LOADER, NULL);
   SetupContextLoaderPrivate *priv = setup_context_loader_get_instance_private(self);

//   priv->clinical_context = clinical_context;
//   priv->maintenance_context = maintenance_context;
//   priv->calibration_context = calibration_context;

//   CommandStep *lander = command_step_new("Clinical Landing page", priv->model);
   PendingStep *pender = pending_step_new("Clinical Landing page", priv->model, "obj-prop-to-bind-to");

   gtk_box_pack_start(GTK_BOX(self->context_box), GTK_WIDGET(pender), TRUE, TRUE, 0);
   gtk_widget_set_visible(GTK_WIDGET(pender), TRUE);

//   gtk_box_pack_end(GTK_BOX(self->context_box), GTK_WIDGET(priv->maintenance_context), TRUE, TRUE, 0);
//   gtk_widget_set_visible(GTK_WIDGET(priv->maintenance_context), FALSE);
//
//   gtk_box_pack_end(GTK_BOX(self->context_box), GTK_WIDGET(priv->calibration_context), TRUE, TRUE, 0);
//   gtk_widget_set_visible(GTK_WIDGET(priv->calibration_context), FALSE);
//
//   priv->current_strategy = clinical_context;

   return self;
}

RUN_MODEL_PHASES setup_context_loader_get_run_phase( SetupContextLoader *self )
{
   g_return_val_if_fail( RUN_IS_MODEL( self ), FALSE );
   return self->current_phase;
}
void setup_context_loader_set_run_phase( SetupContextLoader *self, RUN_MODEL_PHASES phase )
{
   g_return_if_fail( RUN_IS_MODEL( self ) );
   self->current_phase = phase;

   logging_llprintf(LOGLEVEL_DEBUG, "%s: PHASE CHANGE NEED TO UPDATE VIEW?", __func__);
}
