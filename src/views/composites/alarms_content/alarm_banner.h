/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef ALARM_BANNER_H__
#define ALARM_BANNER_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALARM_TYPE_BANNER (alarm_banner_get_type())

G_DECLARE_FINAL_TYPE(AlarmBanner, alarm_banner, ALARM, BANNER, GtkBox)

AlarmBanner *alarm_banner_new(const gchar *test_arg);

void alarm_banner_set_alarm_high(AlarmBanner *self);
void alarm_banner_set_alarm_mid(AlarmBanner *self);
void alarm_banner_set_alarm_low(AlarmBanner *self);
void alarm_banner_set_alarm_off(AlarmBanner *self);

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARM_BANNER_H__
