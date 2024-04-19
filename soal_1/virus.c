#include <dirent.h> // Mengoperasikan direktori
#include <errno.h> // Error handling
#include <fcntl.h> // File control
#include <stdio.h> // Operasi input/output
#include <stdlib.h> // Alokasi memori
#include <string.h> // Manipulasi string
#include <sys/types.h> // Tipe data khusus sistem
#include <sys/stat.h> // Informasi status file
#include <syslog.h> // Mencatat log ke sistem log
#include <time.h> // Manipulasi waktu
#include <unistd.h> // Operasi sistem dasar

void logActivity(const char *dirPath, const char *fileName) {
    // Fungsi untuk mencatat aktivitas ke file log
    // dirPath, path direktori tempat file berada
    // fileName, nama file yang sedang diproses
    
    FILE *logFile = fopen("/home/dian/Documents/a/virus.log", "a"); 
    // Buka file log (virus.log) di direktori. Mode "a" berarti buka file dalam mode append, jadi kita tambahkan data baru ke akhir file yang sudah ada. Isi file sebelumnya tetap aman, dan data baru masuk di bagian akhir. Jika file (virus.log) belum ada, program akan membuatnya otomatis
    
    time_t currentTime; 
    // Variabel untuk menyimpan waktu saat ini (timestamp)
    struct tm *localTime; 
    // Variabel untuk menyimpan waktu dalam format waktu lokal
    time(&currentTime); 
    // Mengambil waktu saat ini dan menyimpannya di currentTime
    localTime = localtime(&currentTime); 
    // Mengonversi waktu saat ini (currentTime) ke format waktu lokal dan menyimpannya di localTime

    char timeString[100];
    strftime(timeString, sizeof(timeString), "[%d-%m-%Y][%H:%M:%S]", localTime);
    // Ubah waktu lokal jadi string dengan format [dd-mm-yyyy][hh:mm:ss] dan simpan ke timeString
    
    fprintf(logFile, "%s Suspicious string at %s/%s successfully replaced!\n", timeString, dirPath, fileName);
    // Tulis catatan ke file log, berisi waktu, direktori, dan nama file yang dimodifikasi
    fclose(logFile);
    // Tutup file log setelah selesai
}

char *replaceString(char *str, char *oldWord, char *newWord) {
// Fungsi untuk mengganti string dalam teks
    char *result, *temp, *start = str; 
    // Variabel untuk menyimpan string hasil, posisi sementara, dan posisi awal teks
    int oldWordLen = strlen(oldWord); 
    // Panjang (oldWord) yang ingin diganti
    int newWordLen = strlen(newWord); 
    // Panjang (newWord) yang akan menggantikan (oldWord)
    int length = 0; 
    // Variabel penghitung panjang teks hasil penggantian
    
while ((temp = strstr(start, oldWord))) {
// Menghitung panjang teks hasil setelah penggantian kata
    length += temp - start;
    // Tambahkan panjang teks dari awal hingga posisi kata lama (oldWord)
    start = temp + oldWordLen;
    // Geser posisi awal ke setelah (oldword) untuk melanjutkan pencarian
    length += newWordLen;
    // Tambahkan panjang (newWord) ke panjang total teks hasil
}
length += strlen(start) + 1;
// Tambahkan panjang sisa teks setelah posisi awal saat ini hingga akhir teks
    result = (char *)malloc(length);
    // Alokasi memori untuk teks hasil dengan panjang yang sudah dihitung menggunakan malloc
    start = str;
    // Kembali ke awal teks yang akan diproses
    *result = '\0';
    // Inisialisasi teks hasil dengan mengosongkan string (menetapkan karakter null '\0')
    
    while ((temp = strstr(start, oldWord))) {
    // Ganti (oldWord) dengan (newWord) di dalam teks
        strncat(result, start, temp - start); 
        // Salin teks dari awal hingga posisi (oldWord) dalam hasil
        strcat(result, newWord); 
        // Tambahkan (newWord) ke dalam hasil
        start = temp + oldWordLen; 
        // Geser posisi awal untuk melanjutkan setelah (oldWord) yang ditemukan
    }
    
    strcat(result, start);
    // Tambahkan sisa teks dari posisi awal saat ini (start) ke hasil
    return result; 
    // Kembalikan teks hasil setelah semua penggantian kata selesai
}

void processFile(const char *dirPath, const char *fileName) {
// Fungsi untuk memproses file dalam direktori
    char filePath[1024]; 
    // Deklarasikan array filePath untuk menyimpan path lengkap file, dengan ukuran maksimum 1024 karakter
    sprintf(filePath, "%s/%s", dirPath, fileName); 
    // Gabungkan path direktori (dirPath) dan nama file (fileName) menjadi path lengkap (filePath)
    
    FILE *file = fopen(filePath, "r"); 
    // Buka file dalam mode baca
    
    if (file != NULL) {
    // Jika file berhasil dibuka (tidak bernilai NULL):
        char buffer[1024]; 
        // Buat buffer untuk menyimpan data yang dibaca dari file
        fread(buffer, 1, sizeof(buffer), file); 
        // Baca isi file ke buffer
        fclose(file); 
        // Tutup file setelah baca
        
        char *modifiedContent = strdup(buffer); 
        // Buat salinan isi buffer untuk dimodifikasi
        modifiedContent = replaceString(modifiedContent, "m4LwAr3", "[MALWARE]"); 
        // Ganti string "m4LwAr3" dengan "[MALWARE]"
        modifiedContent = replaceString(modifiedContent, "5pYw4R3", "[SPYWARE]"); 
        // Ganti string "5pYw4R3" dengan "[SPYWARE]"
        modifiedContent = replaceString(modifiedContent, "R4nS0mWaR3", "[RANSOMWARE]"); 
        // Ganti string "R4nS0mWaR3" dengan "[RANSOMWARE]"
        
        if (strcmp(buffer, modifiedContent) != 0) {
        // Jika isi file berbeda dengan konten yang dimodifikasi:
            file = fopen(filePath, "w"); 
            // Buka file dalam mode tulis (w), yang akan menimpa isi file yang ada
            fputs(modifiedContent, file); 
            // Tulis konten yang sudah dimodifikasi ke dalam file
            fclose(file); 
            // Tutup file setelah menulis
            logActivity(dirPath, fileName); 
            // Catat aktivitas modifikasi di log
            sleep(15); 
            // Sleep 15 detik sebelum lanjut
        }
        free(modifiedContent); 
        // Bebaskan memori yang digunakan untuk menyimpan konten modifikasi
    }
}

void traverseDirectory(const char *dirPath) {
// Fungsi untuk menelusuri direktori yang diberikan
    struct dirent *entry; 
    // Variabel untuk menyimpan entri direktori saat membaca isinya
    DIR *directory = opendir(dirPath); 
    // Buka direktori
    
    if (directory != NULL) {
    // Jika direktori berhasil dibuka (tidak NULL):
        while ((entry = readdir(directory)) != NULL) {
        // Baca entri direktori satu per satu
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Lewati entri "." (direktori saat ini) dan ".." (direktori sebelumnya)
                char path[1024]; 
                // Path lengkap
                sprintf(path, "%s/%s", dirPath, entry->d_name); 
                // Gabungkan dirPath dan nama entry direktori
                
                struct stat fileStat; 
                // Variabel untuk menyimpan info status file
                if (lstat(path, &fileStat) == 0) {
                    // Memeriksa jenis entri: direktori atau file biasa
                    if (S_ISDIR(fileStat.st_mode)) {
                        traverseDirectory(path); 
                        // Telusuri direktori lebih dalam
                    } else if (S_ISREG(fileStat.st_mode)) {
                    // Jika entri adalah file biasa (bukan direktori atau jenis file lainnya),
                        processFile(dirPath, entry->d_name); 
                        // panggil fungsi processFile untuk memproses file tersebut
                    }
                }
            }
        }
        closedir(directory); 
        // Tutup direktori setelah selesai membaca isinya
    }
}

int main(int argc, char *argv[]) {
    pid_t pid, sid; 
    // Variabel untuk menyimpan PID (proses ID) dan SID (session ID) untuk proses daemon
    
    pid = fork(); 
    // Membuat child process dengan fork
    
    if (pid < 0) {
        // Jika pid < 0, maka pembuatan proses gagal dan program harus keluar
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {
        // Jika pid > 0, berarti proses induk
        exit(EXIT_SUCCESS);
        // Keluar dari proses induk sehingga child process dapat berjalan sebagai daemon
    }
    
    umask(0); 
    // Mengatur permission mask (umask) menjadi 0 sehingga proses daemon dapat membuat file
    // dengan permission penuh
    
    sid = setsid(); 
    // Membuat sesi baru (session ID) untuk proses daemon
    // Mengdetach proses dari terminal sehingga daemon tetap berjalan meskipun terminal ditutup
    
    if (sid < 0) {
        exit(EXIT_FAILURE); 
        // Jika gagal membuat session ID baru (sid), keluar dari program dengan kode kesalahan
    }
    
    if (chdir("/") < 0) {
    // Ubah direktori kerja menjadi root ("/") untuk menghindari masalah dengan perubahan filesystem
        exit(EXIT_FAILURE); 
    }
    
    // Close file standar input/output/error untuk mencegah proses daemon berinteraksi dengan terminal atau menghasilkan output yang tidak diinginkan
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while (1) {
    // Loop utama daemon, akan terus berjalan selamanya
        sleep(15); 
        // Yang membuat program berjalan setiap 15 detik
        traverseDirectory(argv[1]); 
        // Telusuri direktori yang diberikan sebagai argumen (argv[1]), ini akan memulai proses penelusuran direktori
    }
    return 0;
}
