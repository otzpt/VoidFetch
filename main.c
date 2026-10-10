#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
            printf("motherboard: %s", read_model);
            break;
        }
        fclose(file_model);
    }
}

void cpu_name(char *buffer, size_t max_len)
{
    FILE *file = fopen("/proc/cpuinfo", "r");

    if (file == NULL) {
        printf("ERROR: couldn't read file");
    } else {
        char cpu_info[256];
        while(fgets(cpu_info, sizeof(cpu_info), file) != NULL) {
            // look for cpu model
            if (strncmp(cpu_info, "model name", 10) == 0) {
                char *colon = strchr(cpu_info, ':');

                if (colon) {
                    char *name = colon + 1; //skip colon
                    while(*name == ' ' || *name == '\t') { name++; }

                    // strip new char
                    name[strcspn(name, "\n")] = '\0';

                    strncpy(buffer, name, max_len - 1);
                    buffer[max_len - 1] = '\0';

                    break;
                }
            }
        }
        fclose(file);
    }
}

double cpu_clock()
{
    double max_clock = 0.0;
    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");

    if (file == NULL) {
        printf("ERROR: couldn't read file");
    } else {
        char read[256];
        if (fgets(read, sizeof(read), file) != NULL) {
            double clock = strtod(read, NULL);
            max_clock = clock / 1000000.0;
        }
    fclose(file);
    }
    return max_clock;
}

void print_cpu(void) {
    char cpu_model[256] = "Unknown CPU";

    cpu_name(cpu_model, sizeof(cpu_model));
    long threads = sysconf(_SC_NPROCESSORS_CONF);
    double clock = cpu_clock();

    printf("CPU: %s (%ld threads) %.2f GHz\n",cpu_model, threads, clock);
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

struct Hardware_linux {
    void (*motherboard)(void);
    void (*cpu)(void);
    void (*gpu)(void);
    void (*ram)(void);
    void (*disk)(void);
};

int main(void) {
    struct utsname info;
    struct Hardware_linux hw = {
        .motherboard = motherboard,
        .cpu = print_cpu,
        .gpu = NULL,
        .ram = NULL,
        .disk = NULL
    };


    distro_OS();

    // kernel part was already done so no need to make extra function nor assign it to a struct
    uname(&info);
    printf("OS Kernel: %s\n", info.sysname);

    hw.motherboard();
    uptime();

    hw.cpu();

    return 0;
}
