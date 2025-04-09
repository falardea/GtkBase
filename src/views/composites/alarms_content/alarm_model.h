/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef ALARM_MODEL_H__
#define ALARM_MODEL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define ALARM_TYPE_MODEL              ( alarm_model_get_type( ) )

G_DECLARE_FINAL_TYPE(AlarmModel, alarm_model, ALARM, MODEL, GObject)

typedef enum
{
   AM_NO_ALARM = 0,
   AM_BASIC_LOW_ALARM = 1,
   AM_BASIC_MID_ALARM = 2048,
   AM_BASIC_HIGH_ALARM = 4096,
} ALARM_MODEL_LEVEL;


   /*
GType
alarm_model_levels_get_type (void)
{
   static GType etype = 0;
   if (etype == 0) {
      static const GEnumValue values[] = {
            { AM_BASIC_HIGH_ALARM, "AM_BASIC_HIGH_ALARM", "alarm-high" },
            { AM_BASIC_MID_ALARM, "AM_BASIC_MID_ALARM", "alarm-mid" },
            { AM_BASIC_LOW_ALARM, "AM_BASIC_LOW_ALARM", "alarm-low" },
            { 0, NULL, NULL }
      };
      etype = g_enum_register_static("AlarmModelLevels", values);
   }
   return etype;
}
    */

/* Methods for our newly declared GObject derived GType*/
AlarmModel *alarm_model_new();

ALARM_MODEL_LEVEL alarm_model_get_alarm_level( AlarmModel *self );
void alarm_model_set_alarm_level( AlarmModel *self, ALARM_MODEL_LEVEL level );

G_END_DECLS
#endif  // ALARM_MODEL_H__
