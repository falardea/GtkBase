/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef ALARM_BANNER_H__
#define ALARM_BANNER_H__

#include <gtk/gtk.h>
#include "alarm_model.h"

G_BEGIN_DECLS

#define ALARM_TYPE_BANNER (alarm_banner_get_type())

G_DECLARE_FINAL_TYPE(AlarmBanner, alarm_banner, ALARM, BANNER, GtkBox)

AlarmBanner *alarm_banner_new();

void alarm_banner_set_alarm_level(AlarmBanner *self, ALARM_MODEL_LEVEL level);

G_END_DECLS
#endif  // ALARM_BANNER_H__
