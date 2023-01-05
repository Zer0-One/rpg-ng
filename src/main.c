#include <stdio.h>
#include <unistd.h>

void print_usage(){
    printf("Usage: rpg-ng [-d]\n");
    printf("Command-line options:\n");
    printf("\n\t-d,\tEnables debug mode, increasing logging verbosity");
    printf("\n\t-h,\tPrints this help information");
    printf("\n\t-v,\tPrints version information");
    printf("\n");
}

const unsigned int VERSION_MAJOR = RPGNG_VERSION_MAJOR;
const unsigned int VERSION_MINOR = RPGNG_VERSION_MINOR;
const unsigned int VERSION_PATCH = RPGNG_VERSION_PATCH;
const char* const VERSION = RPGNG_VERSION;

void print_versions(){
    printf("rpg-ng version %s\n\n", RPGNG_VERSION);
    printf("Built with support for: \n");
    printf("\tSuper Cool Lib 1.5.2\n");
    printf("\tOther Lib 5.3\n");
}

int main(int argc, char* argv[]){
    int opt;

    while((opt = getopt(argc, argv, "dhv")) != -1){
        switch(opt){
            case 'd':
                printf("Wow very cool debug mode\n");
                break;
            case 'h':
                print_usage();
                _exit(0);
            case 'v':
                print_versions();
                _exit(0);
            case '?':
                print_usage();
                _exit(-1);
        }
    }
}
