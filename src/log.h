// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_LOG
#define RPGNG_LOG

#include <stdio.h>

typedef enum LOG_PRIORITY {
    // Debug information
    LOG_DEBUG,
    // Nothing has gone wrong, but the user should probably know something
    LOG_INFO,
    // Error, but it's recoverable
    LOG_WARN,
    // Error, and it's not recoverable
    LOG_ERR,
    // Call the fire department
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
int log_close(void);

#endif
