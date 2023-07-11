// SPDX-FileCopyrightText: 2023 David Zero <zero-one@zer0-one.net>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef RPGNG_GETOPT
#define RPGNG_GETOPT

/**
 * POSIX getopt() for platforms that don't have it. I wrote this without
 * looking at any POSIX sources, so hopefully it behaves correctly.
 */

#include <stdio.h>
#include <string.h>

char* optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = 0;

int getopt(int argc, char* argv[], const char* optstring) {
    // No more arguments available (second condition should never hit)
    if(optind >= argc || argv[optind] == NULL) {
        return -1;
    }

    // Next option doesn't start with a hyphen
    if(argv[optind][0] != '-') {
        return -1;
    }

    // Next option is a single dash with no letter
    if(argv[optind][1] == '\0') {
        return -1;
    }

    // Next option is double-dash, remaining arguments are positional
    if(argv[optind][1] == '-') {
        optind++;

        return -1;
    }

    // Search optstring for option
    for(size_t i = (optstring[0] == ':' ? 1 : 0); i < strlen(optstring); i++) {
        if(argv[optind][1] == optstring[i]) {
            // This option has an argument
            if(optstring[i + 1] == ':') {
                // Argument is attached to option without space
                if(argv[optind][2] != '\0') {
                    optarg = &argv[optind][2];
                }
                // Option is the last in the list, but we're out of arguments
                else if(optind == argc - 1) {
                    printf("%s: option requires an argument -- '%c'\n", argv[0], argv[optind][1]);

                    if(optstring[0] == ':') {
                        return ':';
                    }

                    return '?';
                }
                // Argument is spaced
                else {
                    optarg = argv[optind + 1];

                    optind++;
                }
            }

            optind++;

            return optstring[i];
        }
    }

    // Option not found in optstring
    if(optstring[0] != ':' && opterr != 0) {
        printf("%s: invalid option -- '%c'\n", argv[0], argv[optind][1]);
    }

    optopt = argv[optind][1];

    return '?';
}

#endif
