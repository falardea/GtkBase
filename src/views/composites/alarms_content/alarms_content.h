/**
 * Created by french on 3/22/25.
 * @brief
 */
#ifndef ALARMS_CONTENT_H__
#define ALARMS_CONTENT_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ALARMS_TYPE_CONTENT (alarms_content_get_type())

G_DECLARE_FINAL_TYPE(AlarmsContent, alarms_content, ALARMS, CONTENT, GtkBox)

AlarmsContent *alarms_content_new();

G_END_DECLS
#endif  // ALARMS_CONTENT_H__
