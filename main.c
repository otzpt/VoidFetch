#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/statvfs.h>

#define MAX_LINES 16
// NOTICE
// if i comment obvious stuff is bc im new
// to this programming stuff so i need guidance on code to
// be able to read it in an easier way
// and helps me debug

void distro_OS(char *out, size_t size)
{
    FILE *file = fopen("/etc/os-release", "r");

    // handle failure to open the file
    if (file == NULL) {
        snprintf(out, size, "Error: couldn't read file");
    } else {
        // read file, clean the "" and print it
        char read[256];
        snprintf(out, size, "OS: unknown");
        while (fgets(read, sizeof(read), file) != NULL) {
            if (strncmp(read, "PRETTY_NAME=", 12) == 0) {
                // removes "" before printing
                read[strcspn(read, "\r\n")] = '\0';
                read[strlen(read) - 1] = '\0';
                snprintf(out, size, "OS: %s", read + 13);
                break;
            }
        }
        fclose(file);
    }
}

void motherboard(char *out, size_t size)
{
    FILE *file_model = fopen("/sys/class/dmi/id/board_name", "r");

    if (file_model == NULL) {
        snprintf(out, size, "ERROR: couldn't read file");
    } else {
        char read_model[256];
        snprintf(out, size, "motherboard: unknown");

        while (fgets(read_model, sizeof(read_model), file_model) != NULL) {
            // removes new chjar, else it breaks the logo align
            read_model[strcspn(read_model, "\n")] = '\0';
            snprintf(out, size, "motherboard: %s", read_model);
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

void print_cpu(char *out, size_t size) {
    char cpu_model[256] = "Unknown CPU";

    cpu_name(cpu_model, sizeof(cpu_model));
    long threads = sysconf(_SC_NPROCESSORS_CONF);
    double clock = cpu_clock();

    snprintf(out, size, "CPU: %s (%ld threads) %.2f GHz",cpu_model, threads, clock);
}

void ram(char *out, size_t size)
{
    FILE *file = fopen("/proc/meminfo", "r");
    double total_ram = 0;
    double used_ram = 0;

    if (file == NULL) {
        snprintf(out, size, "ERROR: couldn't read file");
    } else {
        char read[256];
        snprintf(out, size, "RAM: unknown");

        while (fgets(read, sizeof(read), file) != NULL) {
            if (strncmp(read, "MemTotal", 8) == 0) {
                char *colon = strchr(read, ':');

                if (colon) {
                    char *max_mem = colon + 1;

                    while(*max_mem == ' ' || *max_mem == '\t') { max_mem++; }
                    max_mem[strcspn(max_mem, "\n")] = '\0';

                    total_ram = strtod(max_mem, NULL) / 1024.0 / 1024.0;

                }
            }

            if (strncmp(read, "MemAvailable", 12) == 0) {
                char *colon = strchr(read, ':');

                if (colon) {
                    char *avail_mem = colon + 1;

                    while(*avail_mem == ' ' || *avail_mem == '\t') { avail_mem++; }

                    avail_mem[strcspn(avail_mem, "\n")] = '\0';

                    used_ram = strtod(avail_mem, NULL) / 1024.0 / 1024.0;
                    used_ram = total_ram - used_ram;

                    snprintf(out, size, "RAM: %.1f GiB | %.1f GiB", used_ram, total_ram);

                    break;
                }
            }
        }
        fclose(file);
    }
}


void disk_space(char *out, size_t size) {
    struct statvfs ds;

    if (statvfs("/", &ds) == 0) {
        // f_frsize for accurate byte sizing on linux
        unsigned long long total_space = ds.f_blocks * ds.f_frsize;
        unsigned long long available_space = ds.f_bavail * ds.f_frsize;
        unsigned long long used_space = total_space - available_space;

        snprintf(out, size, "DISK: %llu GiB / %llu GiB", used_space / (1024ULL * 1024 * 1024), total_space / (1024ULL * 1024 * 1024));
    } else {
        snprintf(out, size, "ERROR: couldn't determine disk space");
    }
}

long read_pci_hex(const char *dev, const char *file_name)
{
    char path[512];
    snprintf(path, sizeof(path), "/sys/bus/pci/devices/%s/%s", dev, file_name);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    char read[64];
    long value = -1;

    if (fgets(read, sizeof(read), file) != NULL) {
        value = strtol(read, NULL, 16); // base 16 accepts the "0x" prefxi i think
    }
    fclose(file);
    return value;
}

const char *vendor_name(long id)
{
    switch (id) {
        case 0x10de: return "NVIDIA";
        case 0x1002: return "AMD";
        case 0x8086: return "Intel";
        default: return "Unknown";
    }
}

int gpu_model(long vendor, long device, char *out, size_t size)
{
    const char *paths[] = { "/usr/share/hwdata/pci.ids", "/usr/share/misc/pci.ids" };
    FILE *file = NULL;

    // try each known location until one opens
    for (int i = 0; i < 2 && file == NULL; i++) {
        file = fopen(paths[i], "r");
    }
    if (file == NULL) {
        return 0;
    }

    char read[512];
    int in_vendor = 0; // 1 when we are inside the vendor block
    int found = 0;

    while (!found && fgets(read, sizeof(read), file) != NULL) {
        if (read[0] == '#' || read[0] == '\n') {
            continue;
        }

        char *end;

        if (read[0] != '\t') {
            //vendor line:"10de  NVIDIA corporation"
            // OMG this is so hard
            if (in_vendor) {
                break;
            }

            long id = strtol(read, &end, 16);
            // 4 hex digits, so things like "C 03 ..." get ignored
            in_vendor = (end - read == 4 && id == vendor);
        } else if (in_vendor && read[1] != '\t') {
            // device line: "<tab>1e81 TU104 [GeForce RTX 2080 SUPER]" 2 tabs means a subsystem line
            long id = strtol(read + 1, &end, 16);

            if (end - (read + 1) == 4 && id == device) {
                while (*end == ' ') {end++;}
                end[strcspn(end, "\n")] = '\0';
                snprintf(out, size, "%s", end);
                found = 1;
            }
        }
    }
    fclose(file);
    return found;
}

// writes one line per GPU
// never writes at past lines[max]
// return the amount of lines it wrote
int gpu_detect(char lines[][256], int start, int max)
{
    int count = 0;
    DIR *dir = opendir("/sys/bus/pci/devices/");

    if (dir != NULL) {
        struct dirent *entry;

        while (start + count < max && (entry = readdir(dir))!= NULL) {
            long class_code = read_pci_hex(entry -> d_name, "class");

            // top byte 0x03 is the display controller (VGA, 3D, etc)
            if(class_code < 0 || (class_code >> 16) != 3) {
                continue;
            }

            long vendor = read_pci_hex(entry -> d_name, "vendor");
            long device = read_pci_hex(entry -> d_name, "device");

            if (vendor < 0 || device < 0) {
                continue;
            }

            char model[128];
            if (gpu_model(vendor, device, model, sizeof(model))) {
                snprintf(lines[start + count], 256, "GPU: %s %s", vendor_name(vendor), model);
            } else {
                // no pci.ids on this machine, fall back to the raw ids
                snprintf(lines[start + count], 256, "GPU: %s (%04lx:%04lx)",
                         vendor_name(vendor), vendor, device);
            }

            count++;
        }
        closedir(dir);
    }

    // in case nothing found or sys not readble, still fill a line
    if (count == 0) {
        snprintf(lines[start], 256, "GPU: unknown");
        count = 1;
    }
    return count;
}

void uptime(char *out, size_t size)
{
    FILE *file = fopen("/proc/uptime", "r");

    if (file == NULL) {
        snprintf(out, size, "ERROR: couldn't read file");
    } else {
        char read[50];
        if (fgets(read, sizeof(read), file) != NULL) {
            double seconds = strtod(read, NULL); // converts str to double
            int total_seconds = (int)seconds; // coverts seconds to int

            int hours = total_seconds / 3600;
            int minutes = (total_seconds % 3600) / 60;
            int remaining_seconds = total_seconds % 60;

            snprintf(out, size, "uptime: %dh, %dmin, %ds", hours, minutes, remaining_seconds);
        }
        fclose(file);
    }
}

void shell(char *out, size_t size)
{
    char *path = getenv("SHELL");
    if (path == NULL) {
        snprintf(out, size, "ERROR: failed to identify shell");
    } else {
        const char *slash = strrchr(path, '/');
        const char *shell_name = (slash != NULL) ? slash + 1 : path;
        snprintf(out,size,"SHELL: %s", shell_name);
    }
}

const char *logo(void) {
    #ifdef __linux__
        return
            "             +%@@@%*=\n"
            "           .%@@@@@@#@%-\n"
            "           *@%@@@@%%@@@.\n"
            "           *+#-+--%-*@@-\n"
            "           +#::..::-%@@*\n"
            "           +%-====-:+@*@-\n"
            "        :#@.          #@@@%:\n"
            "       -@@=         ..:#@@@@+\n"
            "       %@=              #%%@@=\n"
            "     #@@.               .@@%@@+\n"
            "    :%##.               :@%@%@=\n"
            "   .:::=#+.            --@@@@*:\n"
            ".-::::::::*%+        :*=::::::::::\n"
            "--:::::::::-::.  .:=#@%-:::::----:\n"
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

char *get_user(void)
{
    // this doesn't need a function but i need to move fast so yea
    char *username = getenv("USER");

    if (username == NULL) {
        username = "UNKNOWN";
    }

    return username;
}

int main(void) {
    int start = 0;
    const char *art = logo();
    struct utsname info;

    // fills info with kernel name, hostname etc
    uname(&info);

    // each row is one line of text, filled at runtime instead of hardcoded
    char lines[MAX_LINES][256] = {0};
    int info_count = 0;

    snprintf(lines[info_count++], 256, "%s@%s", get_user(), info.nodename);
    snprintf(lines[info_count++], 256, "---------------------------------------");
    distro_OS(lines[info_count++], 256);
    snprintf(lines[info_count++], 256, "OS Kernel: %s | %s", info.sysname, info.release);
    shell(lines[info_count++], 256);
    uptime(lines[info_count++], 256);
    motherboard(lines[info_count++], 256);
    print_cpu(lines[info_count++], 256);
    ram(lines[info_count++], 256);
    info_count += gpu_detect(lines, info_count, MAX_LINES - 1);
    disk_space(lines[info_count++], 256);

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
                printf("  %s", lines[row]);
            }

            printf("\n");

            start = i + 1;
            row++;
        }
    }

    return 0;
}
