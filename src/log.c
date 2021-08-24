#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>
#include <time.h>

#include "config.h"

#define DATE_BUFFER_SIZE 100

void logmsg(int loglevel, char* msg, ...){
    if((loglevel == LOG_DEBUG || loglevel == LOG_INFO || loglevel == LOG_NOTICE) && !config_log_debug){
        return;
    }

    va_list args;
    va_start(args, msg);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    char date_buffer[DATE_BUFFER_SIZE];
    struct tm* bd_time = localtime(&ts.tv_sec);
    strftime(date_buffer, DATE_BUFFER_SIZE, "[%F %T]", bd_time);

    if(loglevel == LOG_ALERT){
        printf("%s ALERT ", date_buffer);
    }
    else if(loglevel == LOG_CRIT){
        printf("%s CRITICAL ", date_buffer);
    }
    else if(loglevel == LOG_DEBUG){
        printf("%s DEBUG ", date_buffer);
    }
    else if(loglevel == LOG_EMERG){
        printf("%s EMERGENCY ", date_buffer);
    }
    else if(loglevel == LOG_ERR){
        printf("%s ERROR ", date_buffer);
    }
    else if(loglevel == LOG_INFO){
        printf("%s INFO ", date_buffer);
    }
    else if(loglevel == LOG_NOTICE){
        printf("%s NOTICE ", date_buffer);
    }
    else if(loglevel == LOG_WARNING){
        printf("%s WARNING ", date_buffer);
    }
    
    vprintf(msg, args);
    printf("\n");

    va_end(args);

    fflush(NULL);
}
