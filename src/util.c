#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

int cmpver(const char* ver1, const char* ver2){
    size_t major = 0, minor = 0, patch = 0;
    size_t major2 = 0, minor2 = 0, patch2 = 0;

    errno = 0;

    int ret = sscanf(ver1, "%zu.%zu.%zu", &major, &minor, &patch);
    int ret2 = sscanf(ver2, "%zu.%zu.%zu", &major2, &minor2, &patch2);

    if(ret < 1 || ret2 < 1){
        if(errno == 0){
            logmsg(LOG_WARNING, "Encountered malformed version string during comparison between '%s' and '%s'", ver1, ver2);
            
            return INT_MAX;
        }

        logmsg(LOG_WARNING, "Unable to compare versions '%s' and '%s', %s", ver1, ver2, strerror(errno));

        return INT_MAX;
    }

    if(major == major2 && minor == minor2 && patch == patch2){
        return 0;
    }

    if(major > major2){
        return 1;
    }

    if(major2 > major){
        return -1;
    }

    if(minor > minor2){
        return 1;
    }

    if(minor2 > minor){
        return -1;
    }

    if(patch > patch2){
        return 1;
    }

    return -1;
}

char* strstrp(const char* str, char chr){
    if(str == NULL){
        return NULL;
    }

    size_t str_len = strlen(str);

    size_t chr_count = 0;

    size_t i = 0;
    while(true){
        if(str[i] == chr){
            chr_count++;
        }
        else if(str[i] == 0){
            break;
        }

        i++;
    }

    size_t out_len = str_len - chr_count + 1;
    char* out = malloc(out_len);

    size_t j = 0;
    for(i = 0; i < str_len; i++){
        if(str[i] != chr){
            out[j] = str[i];
            j++;
        }
    }

    out[out_len - 1] = 0;

    return out;
}
