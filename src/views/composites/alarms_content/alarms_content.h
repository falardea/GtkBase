/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef ALARMS_CONTENT_H__
#define ALARMS_CONTENT_H__

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALARMS_TYPE_CONTENT (alarms_content_get_type())

G_DECLARE_FINAL_TYPE(AlarmsContent, alarms_content, ALARMS, CONTENT, GtkBox)

AlarmsContent *alarms_content_new();

#ifdef __cplusplus
}  // closing brace for extern "C"
#endif
#endif  // ALARMS_CONTENT_H__
