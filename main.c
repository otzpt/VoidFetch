#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>

// NOTICE
// if i comment obvious stuff is bc im new
// to this programming stuff so i need guidance on code to
// be able to read it in an easier way
// and helps me debug

void distro_OS()
{
    FILE *file = fopen("/etc/os-release", "r");

    // handle failure to open the file
    if (file == NULL) {
        printf("Error: couldn't read file\n");
    } else {
        // read file, clean the "" and print it
        char read[256];
        while (fgets(read, sizeof(read), file) != NULL) {
            if (strncmp(read, "PRETTY_NAME=", 12) == 0) {
                read[strlen(read) - 2] = '\0';
                printf("OS: %s\n", read + 13);
                break;
            }
        }
        fclose(file);
    }
}

void motherboard()
{
    FILE *file_model = fopen("/sys/class/dmi/id/board_name", "r");

    if (file_model == NULL) {
        printf("ERROR: couldn't read file\n");
    } else {
        char read_model[256];
        while (fgets(read_model, sizeof(read_model), file_model) != NULL) {
            printf("motherboard: %s\n", read_model);
            break;
        }
        fclose(file_model);
    }
}

void uptime()
{
    FILE *file = fopen("/proc/uptime", "r");

    if (file == NULL) {
        printf("ERROR: couldn't read file\n");
    } else {
        char read[50];
        if (fgets(read, sizeof(read), file) != NULL) {
            double seconds = strtod(read, NULL); // converts str to double
            int total_seconds = (int)seconds; // coverts seconds to int

            int hours = total_seconds / 3600;
            int minutes = (total_seconds % 3600) / 60;
            int remaining_seconds = total_seconds % 60;

            printf("uptime: %dh, %dmin, %ds\n", hours, minutes, remaining_seconds);
        }
        fclose(file);
    }
}

int main(void) {
    struct utsname info;
    uname(&info);
    printf("OS Kernel: %s\n", info.sysname);

    distro_OS();
    motherboard();
    uptime();

    return 0;
}
