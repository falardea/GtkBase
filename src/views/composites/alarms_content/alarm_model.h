/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef ALARM_MODEL_H__
#define ALARM_MODEL_H__

#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALARM_TYPE_MODEL              ( alarm_model_get_type( ) )

G_DECLARE_FINAL_TYPE(AlarmModel, alarm_model, ALARM, MODEL, GObject)

/* Methods for our newly declared GObject derived GType*/
AlarmModel *alarm_model_new();

gchar *alarm_model_get_name( AlarmModel *self );
void alarm_model_set_name( AlarmModel *self, const gchar *name );

gboolean alarm_model_get_enabled( AlarmModel *self );
void alarm_model_set_enabled( AlarmModel *self, gboolean enabled );

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARM_MODEL_H__
