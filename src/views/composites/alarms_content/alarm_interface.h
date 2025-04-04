/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef ALARM_INTERFACE_H__
#define ALARM_INTERFACE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

typedef enum {
   ALARM_INTERFACE_OFF_ALARM,
   ALARM_INTERFACE_LOW_ALARM,
   ALARM_INTERFACE_MID_ALARM,
   ALARM_INTERFACE_HIGH_ALARM,
   ALARM_INTERFACE_N_ALARMS_LEVELS,
}ALARM_INTERFACE_ALARM_LEVELS;

#define ALARM_TYPE_INTERFACE (alarm_interface_get_type())

G_DECLARE_FINAL_TYPE(AlarmInterface, alarm_interface, ALARM, INTERFACE, GtkBox)

AlarmInterface *alarm_interface_new();

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARM_INTERFACE_H__
