/**
 * Created by french on 3/23/25.
 * @brief
 */
#ifndef ALARM_INTERFACE_H__
#define ALARM_INTERFACE_H__
#include <gtk/gtk.h>
G_BEGIN_DECLS

#define ALARM_TYPE_INTERFACE (alarm_interface_get_type())

G_DECLARE_FINAL_TYPE(AlarmInterface, alarm_interface, ALARM, INTERFACE, GtkBox)

AlarmInterface *alarm_interface_new();

G_END_DECLS
#endif  // ALARM_INTERFACE_H__
