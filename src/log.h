#ifndef GAM_LOG
#define GAM_LOG

#include <stdbool.h>
#include <syslog.h>

/**
 * Logs messages to the foreground.
 */
void logmsg(int loglevel, char* msg, ...);

#endif
