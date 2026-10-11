#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/statvfs.h>

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
                // removes "" before printing
                read[strcspn(read, "\r\n")] = '\0';
                read[strlen(read) - 1] = '\0';
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

                    if (max_len != 0 && buffer != NULL) {
                        strncpy(buffer, name, max_len - 1);
                        buffer[max_len - 1] = '\0';
                    }

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

double ram()
{
    FILE *file = fopen("/proc/meminfo", "r");
    double ram = 0;

    if (file == NULL) {
        printf("ERROR: couldn't read filee");
    } else {
        char read[256];

        while (fgets(read, sizeof(read), file) != NULL) {
            if (strncmp(read, "MemTotal", 8) == 0) {
                char *colon = strchr(read, ':');

                if (colon) {
                    char *max_mem = colon + 1;

                    while(*max_mem == ' ' || *max_mem == '\t') { max_mem++; }
                    max_mem[strcspn(max_mem, "\n")] = '\0';

                    ram = strtod(max_mem, NULL) / 1024.0 / 1024.0;

                    break;
                }
            }
        }
        fclose(file);
    }
    return ram;
}

void disk_space() {
    struct statvfs ds;

    if (statvfs("/", &ds) == 0) {
        // f_frsize for accurate byte sizing on linux
        unsigned long long total_space = ds.f_blocks * ds.f_frsize;
        unsigned long long available_space = ds.f_bavail * ds.f_frsize;
        unsigned long long used_space = total_space - available_space;

        printf("DISK: %llu GiB / %llu GiB \n", used_space / (1024ULL * 1024 * 1024), total_space / (1024ULL * 1024 * 1024));
    } else {
        puts("ERROR: couldn't determine disk space");
    }
}

void gpu_detect()
{
    FILE *file = popen("lspci | grep -E 'VGA|3D' | sed 's/.*: //'", "r");

    if (file != NULL ) {
        char gpu[256];
        if (fgets(gpu, sizeof(gpu), file) != NULL) {
            printf("GPU: %s", gpu);
        }
    pclose(file);
    } else {
        puts("ERROR: couldn't identify GPU");
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

const char *logo(void) {
    #ifdef __linux__
        return
            "             +%%@@@%%*=\n"
            "           .%%@@@@@@#@%%-\n"
            "           *@%%@@@@%%%%@@@.\n"
            "           **==@#-+:#@@-\n"
            "           *+#-+--%%-*@@-\n"
            "           +#::..::-%%@@*\n"
            "           +%%-====-:+@*@-\n"
            "          -%%:.--:.   +@@@=\n"
            "        :#@.          #@@@%%:\n"
            "       -@@=         ..:#@@@@+\n"
            "       %%@=              #%%%%@@=\n"
            "      #@*               :@@%%@@.\n"
            "     #@@.               .@@%%@@+\n"
            "    :%%##.               :@%%@%%@=\n"
            "   .:::=#+.            --@@@@*:\n"
            ".-:-::::-#@#:         .-:+**=::.\n"
            ".-::::::::*%%+        :*=::::::::::\n"
            "--:::::::::-::.  .:=#@%%-:::::----:\n"
            "--------:::=#%%#**#%%@@@#=---==-:\n"
            "    ..:-==++:       .:-***+-\n";
    #elif defined(_WIN32)
        return
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n"
            "################  ################\n";
    #else
        return "unknown\n";
    #endif
}

int main(void) {
    int start = 0;
    const char *art = logo();
    struct utsname info;

    const char *info_lines[] = {
        "OS: Ubuntu 26.04.1 LTS",
        "OS Kernel: Linux",
        "uptime: 46h, 35min, 13s",
        "motherboard: X870E GAMING PLUS WIFI",
        "CPU: AMD Ryzen 5 7600X",
        "RAM: 29.7 GiB",
        "GPU: RTX 2080 SUPER",
        "DISK: 306 GiB / 770 GiB"
    };

    int info_count = sizeof(info_lines) / sizeof(info_lines[0]);
    int row = 0;

    for (int i = 0; art[i] != '\0'; i++) {
        if (art[i] == '\n') {
            // memcpy() copies bytes between memory locations.
            // art + start points to where the current line begins.
            // length tells it how many bytes to copy.
            // n_line[length] = '\0' terminates the string.

            int length = i - start;
            char n_line[length + 1];

            memcpy(n_line, art + start, length);
            n_line[length] = '\0';

            printf("%-38s", n_line);

            if (row < info_count) {
                printf("  %s", info_lines[row]);
            }

            printf("\n");

            start = i + 1;
            row++;
        }
    }

    return 0;
}
