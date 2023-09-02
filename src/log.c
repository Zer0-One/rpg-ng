// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include <time.h>

#include "log.h"
#include "win_compat.h"

#define ERR_BUF_SIZE 1024

FILE* log_file = NULL;
char* log_file_path = NULL;

log_priority log_level = LOG_INFO;

const char* log_prio_str[] = {
    [LOG_DEBUG] = "DEBUG",
    [LOG_INFO] = "INFO",
    [LOG_WARN] = "WARNING",
    [LOG_ERR] = "ERROR",
    [LOG_CRIT] = "CRITICAL",
};

int log_init(log_priority level, char* path) {
    char err_buf[ERR_BUF_SIZE];

    if (path != NULL) {
        FILE* handle = fopen(path, "ab");

        if (!handle) {
            logmsg(LOG_ERR, "log: Unable to open logfile '%s' for writing, %s", path, strerror_r(errno, err_buf, ERR_BUF_SIZE));

            return -1;
        }

        log_file = handle;
        log_file_path = path;
    }

    log_level = level;

    return 0;
}

void logmsg(log_priority loglevel, char* msg, ...) {
    if (loglevel == LOG_DEBUG && log_level != LOG_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, msg);

    time_t cur_time;

#ifdef _MSC_VER
    time(&cur_time);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    cur_time = ts.tv_sec;
#endif

    struct tm bd_time;
    localtime_r(&cur_time, &bd_time);

    char date_buffer[100];
    strftime(date_buffer, 100, "[%F %T]", &bd_time);

    printf("%s %s ", date_buffer, log_prio_str[loglevel]);
    if (log_file != NULL)
        fprintf(log_file, "%s %s ", date_buffer, log_prio_str[loglevel]);

    vprintf(msg, args);
    printf("\n");

    if (log_file != NULL) {
        vfprintf(log_file, msg, args);
        fprintf(log_file, "\n");
    }

    va_end(args);

    fflush(NULL);
}

int log_close(void) {
    char err_buf[ERR_BUF_SIZE];

    if (log_file != NULL) {
        if (fclose(log_file) != 0) {
            logmsg(LOG_ERR, "log: Unable to close logfile '%s', %s", log_file_path, strerror_r(errno, err_buf, ERR_BUF_SIZE));

            return -1;
        }
    }

    return 0;
}
