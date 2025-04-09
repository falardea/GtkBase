/**
 * @brief A modules to centralize interfacing with the OS
 */
#ifndef SYS_INTERFACE_H__
#define SYS_INTERFACE_H__
#include <stdbool.h>
#include "app_globals.h"
G_BEGIN_DECLS

RVALUE set_display_env(void);

void get_timestamp(char *buf, size_t bufsz);

G_END_DECLS
#endif  /* SYS_INTERFACE_H__ */
