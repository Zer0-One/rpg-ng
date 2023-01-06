#ifndef RPGNG_LOG
#define RPGNG_LOG

#include <stdio.h>

typedef enum LOG_PRIORITY{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_CRIT,
} log_priority;

/**
 * Opens a logfile for writing.
 *
 * @return A file handle for the opened logfile.
 */
int log_init(log_priority level, char* path);

/**
 * Logs a message to the foreground and to a logfile (if one is specified).
 *
 * @param loglevel
 * @param msg
 * @param ...
 */
void logmsg(log_priority loglevel, char* msg, ...);

/**
 * Closes the open log file, if any.
 */
int log_close();

#endif
