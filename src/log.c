#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "log.h"

FILE* log_file = NULL;
char* log_file_path = NULL;

log_priority log_level = LOG_INFO;

int log_init(log_priority level, char* path) {
    if(path != NULL) {
        FILE* handle = fopen(path, "ab");

        if(!handle) {
            logmsg(LOG_ERR, "log: Unable to open logfile '%s' for writing, %s", path, strerror(errno));

            return -1;
        }

        log_file = handle;
        log_file_path = path;
    }

    log_level = level;

    return 0;
}

void logmsg(log_priority loglevel, char* msg, ...) {
    if(loglevel == LOG_DEBUG && log_level != LOG_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, msg);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm* bd_time = localtime(&ts.tv_sec);

    char date_buffer[100];
    strftime(date_buffer, 100, "[%F %T]", bd_time);

    if(loglevel == LOG_CRIT) {
        printf("%s CRITICAL ", date_buffer);

        if(log_file != NULL) {
            fprintf(log_file, "%s CRITICAL ", date_buffer);
        }
    } else if(loglevel == LOG_DEBUG) {
        printf("%s DEBUG ", date_buffer);

        if(log_file != NULL) {
            fprintf(log_file, "%s DEBUG ", date_buffer);
        }
    } else if(loglevel == LOG_ERR) {
        printf("%s ERROR ", date_buffer);

        if(log_file != NULL) {
            fprintf(log_file, "%s ERROR ", date_buffer);
        }
    } else if(loglevel == LOG_INFO) {
        printf("%s INFO ", date_buffer);

        if(log_file != NULL) {
            fprintf(log_file, "%s INFO ", date_buffer);
        }
    } else if(loglevel == LOG_WARN) {
        printf("%s WARNING ", date_buffer);

        if(log_file != NULL) {
            fprintf(log_file, "%s WARNING ", date_buffer);
        }
    }

    vprintf(msg, args);
    printf("\n");

    if(log_file != NULL) {
        vfprintf(log_file, msg, args);
        fprintf(log_file, "\n");
    }

    va_end(args);

    fflush(NULL);
}

int log_close() {
    if(log_file != NULL) {
        if(fclose(log_file) != 0) {
            logmsg(LOG_ERR, "log: Unable to close logfile '%s', %s", log_file_path, strerror(errno));

            return -1;
        }
    }

    return 0;
}
