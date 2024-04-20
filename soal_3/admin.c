#include <stdio .h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LOG_FILE_EXTENSION ".log"

// menulis log ke file
void write_log(const char *user, const char *activity, bool success) {
    time_t current_time;
    struct tm *time_info;
    char time_str[20];
    char log_filename[50];
    FILE *log_file;

    // waktu saat ini
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%d:%m:%Y-%H:%M:%S", time_info);

    // membuat nama file log sesuai dengan user
    snprintf(log_filename, sizeof(log_filename), "%s%s", user, LOG_FILE_EXTENSION);

    // membuka file log
    log_file = fopen(log_filename, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    // menulis log ke file
    if (success) {
        fprintf(log_file, "[%s]-pid_kegiatan-%s_JALAN\n", time_str, activity);
    } else {
        fprintf(log_file, "[%s]-pid_kegiatan-%s_GAGAL\n", time_str, activity);
    }

    // menutup file log
    fclose(log_file);
}

// memantau kegiatan user
void monitor_user_activity(const char *user) {
    char activity[100];

    // Loop untuk memantau kegiatan user
    while (1) {
        // Misalnya, kita dapat membaca kegiatan user dari input atau sumber lainnya
        // Di sini, untuk contoh, kita menggunakan kegiatan statis
        strcpy(activity, "Example activity");

        // mencatat kegiatan user ke dalam log
        write_log(user, activity, true);

        // Tunggu beberapa detik sebelum memantau kegiatan berikutnya
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <command> <user>\n", argv[0]);
        printf("Commands:\n");
        printf("  -m : Monitor user activity\n");
        printf("  -s : Stop monitoring user activity\n");
        printf("  -c : Cancel user activities\n");
        printf("  -a : Allow user activities\n");
        return EXIT_FAILURE;
    }

    char *command = argv[1];
    char *user = argv[2];

    if (strcmp(command, "-m") == 0) {
        // Fitur monitor
        printf("Monitoring user: %s\n", user);
        // Mulai memantau kegiatan user
        monitor_user_activity(user);
    } else if (strcmp(command, "-s") == 0) {
        // Fitur stop monitoring
        printf("Stop monitoring user: %s\n", user);
        // Implementasi stop monitoring user
    } else if (strcmp(command, "-c") == 0) {
        // Fitur cancel user activities
        printf("Cancel activities for user: %s\n", user);
        // Implementasi pembatalan kegiatan user
        // Catat pembatalan kegiatan ke dalam log file
        // Format log: [dd:mm:yyyy]-[hh:mm:ss]-pid_kegiatan-nama_kegiatan_GAGAL
        write_log(user, "User sedang aktif", false);
    } else if (strcmp(command, "-a") == 0) {
        // Fitur allow user activities
        printf("Allow activities for user: %s\n", user);
        // Implementasi izinkan kegiatan user kembali
        write_log(user, "User sedang aktif", true);
    } else {
        printf("Unknown command: %s\n", command);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
