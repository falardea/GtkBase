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

#define ALARM_TYPE_INTERFACE (alarm_interface_get_type())

G_DECLARE_FINAL_TYPE(AlarmInterface, alarm_interface, ALARM, INTERFACE, GtkBox)

AlarmInterface *alarm_interface_new();

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARM_INTERFACE_H__
