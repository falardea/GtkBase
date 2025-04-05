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

typedef enum
{
   AM_NO_ALARM = 0,
   AM_BASIC_LOW_ALARM = 1,
   AM_BASIC_MID_ALARM = 2048,
   AM_BASIC_HIGH_ALARM = 4096,
} ALARM_MODEL_LEVEL;

/* Methods for our newly declared GObject derived GType*/
AlarmModel *alarm_model_new();

ALARM_MODEL_LEVEL alarm_model_get_alarm_level( AlarmModel *self );
void alarm_model_set_alarm_level( AlarmModel *self, ALARM_MODEL_LEVEL level );

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARM_MODEL_H__
