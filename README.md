# Sisop-2-2024-MH-IT29
## Anggota Kelompok:
- Dian Anggraeni Putri (5027231016)
- Amoes Noland (5027231028)
- Malvin Putra Rismahardian (5027231048)

## Soal 1
> Dikerjakan oleh: Dian Anggraeni Putri (5027231016)

### 1a
Saya membuat program menerima path direktori sebagai input melalui argumen argv[1]. Path ini menentukan folder di mana program akan menelusuri file-file untuk dieksekusi. Bagian ini berada di fungsi main():
 ```sh
int main(int argc, char *argv[]) {
    // Mengecek apakah argumen path diberikan
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Memulai loop utama daemon
    while (1) {
        sleep(15);
        traverseDirectory(argv[1]); // Menelusuri direktori yang diberikan melalui argv[1]
    }

    return 0;
}
 ```

### 1b
Dalam program ini, ada beberapa string yang perlu diganti berdasarkan ketentuan soal:
- String "m4LwAr3" diganti dengan "[MALWARE]".
- String "5pYw4R3" diganti dengan "[SPYWARE]".
- String "R4nS0mWaR3" diganti dengan "[RANSOMWARE]".
Untuk mengganti string, saya menggunakan fungsi replaceString(). Fungsi ini menerima teks asli, kata yang ingin diganti (oldWord), dan kata pengganti (newWord). Fungsi ini mengembalikan teks yang telah dimodifikasi dengan kata pengganti. Potongan kode fungsi replaceString():
 ```sh
char *replaceString(char *str, char *oldWord, char *newWord) {
    char *result, *temp, *start = str;
    int oldWordLen = strlen(oldWord);
    int newWordLen = strlen(newWord);
    int length = 0;

    // Menghitung panjang teks hasil setelah penggantian kata
    while ((temp = strstr(start, oldWord))) {
        length += temp - start;
        start = temp + oldWordLen;
        length += newWordLen;
    }
    length += strlen(start) + 1;

    // Alokasi memori untuk hasil modifikasi
    result = (char *)malloc(length);
    start = str;
    *result = '\0';

    // Melakukan penggantian kata
    while ((temp = strstr(start, oldWord))) {
        strncat(result, start, temp - start);
        strcat(result, newWord);
        start = temp + oldWordLen;
    }

    // Tambahkan sisa teks
    strcat(result, start);
    return result;
}
 ```

Lalu, fungsi processFile() bertugas membaca isi file, mengganti string dalam file sesuai dengan ketentuan, dan menulis kembali konten yang telah dimodifikasi jika ada perubahan. Potongan kode fungsi processFile():
 ```sh
void processFile(const char *dirPath, const char *fileName) {
    char filePath[1024];
    sprintf(filePath, "%s/%s", dirPath, fileName);

    // Membuka file dalam mode baca
    FILE *file = fopen(filePath, "r");
    if (file != NULL) {
        char buffer[1024];
        fread(buffer, 1, sizeof(buffer), file);
        fclose(file);

        // Membuat salinan isi buffer untuk dimodifikasi
        char *modifiedContent = strdup(buffer);
        modifiedContent = replaceString(modifiedContent, "m4LwAr3", "[MALWARE]");
        modifiedContent = replaceString(modifiedContent, "5pYw4R3", "[SPYWARE]");
        modifiedContent = replaceString(modifiedContent, "R4nS0mWaR3", "[RANSOMWARE]");

        // Jika ada perubahan, tulis kembali ke file
        if (strcmp(buffer, modifiedContent) != 0) {
            file = fopen(filePath, "w");
            fputs(modifiedContent, file);
            fclose(file);
            logActivity(dirPath, fileName);
        }

        // Bebaskan memori
        free(modifiedContent);
    }
}
 ```

### 1c
Program dijalankan sebagai daemon, artinya berjalan di latar belakang tanpa interaksi dengan terminal. Program menggunakan fork untuk membuat child process, sedangkan parent process keluar untuk memberikan jalan bagi child process menjadi daemon. Potongan kode pengaturan daemon:
 ```sh
int main(int argc, char *argv[]) {
    pid_t pid = fork(); // Membuat proses child

    if (pid < 0) {
        exit(EXIT_FAILURE); // Gagal membuat proses
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Keluar dari proses induk
    }

    umask(0); // Mengatur umask untuk izin penuh
    sid = setsid(); // Membuat sesi baru untuk proses daemon

    if (sid < 0) {
        exit(EXIT_FAILURE); // Gagal membuat sesi baru
    }

    if (chdir("/") < 0) {
        exit(EXIT_FAILURE); // Gagal mengubah direktori kerja
    }

    // Tutup input/output/error standar
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Loop utama daemon
    while (1) {
        sleep(15);
        traverseDirectory(argv[1]);
    }

    return 0;
}
 ```

### 1d
Program berjalan dalam loop utama yang berulang terus menerus. Setiap 15 detik, program akan sleep menggunakan sleep(15), kemudian melanjutkan penelusuran direktori dengan memanggil traverseDirectory(argv[1]). Hal ini memungkinkan program berjalan berkala setiap 15 detik. Potongan kode loop utama dalam main():
 ```sh
while (1) {
    sleep(15);
    traverseDirectory(argv[1]); // Telusuri direktori setiap 15 detik
}
 ```

### 1e
Setiap kali program mengganti string dalam file, aktivitas tersebut dicatat dalam virus.log. Fungsi logActivity() digunakan untuk mencatat aktivitas ini dalam format waktu [dd-mm-YYYY][HH:MM:SS], diikuti dengan pesan bahwa string mencurigakan pada file tertentu telah berhasil diganti. Potongan kode fungsi logActivity():
 ```sh
void logActivity(const char *dirPath, const char *fileName) {
    FILE *logFile = fopen("/home/dian/Documents/a/virus.log", "a");

    // Dapatkan waktu saat ini
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);

    // Format waktu
    char timeString[100];
    strftime(timeString, sizeof(timeString), "[%d-%m-%Y][%H:MM:SS]", localTime);

    // Tulis catatan ke log
    fprintf(logFile, "%s Suspicious string at %s/%s successfully replaced!\n", timeString, dirPath, fileName);
    fclose(logFile);
}
 ```
###  Screenshot Hasil Pengerjaan
Sebelum:
![alt text](https://cdn.discordapp.com/attachments/1223171682500350062/1233749012952121478/Screenshot_1647.png?ex=662e39ce&is=662ce84e&hm=5a817082fe27e7a24abef9ae2659c57e8e9b44cee4aa82afec6c7a1083b96303&)
Sesudah:
![alt text](https://cdn.discordapp.com/attachments/1223171682500350062/1233749079507468379/Screenshot_1646.png?ex=662e39de&is=662ce85e&hm=103b15e99b702e967e24fd9c60564abd7f25df404b974c55dc5a4b050b80918c&)
![alt text](https://cdn.discordapp.com/attachments/1223171682500350062/1233749119932170240/Screenshot_1645.png?ex=662e39e8&is=662ce868&hm=84bfc63793abdb033d81a56fa9b9c08d4ff27d2528b298aa7d058f743a0367e5&)

## Soal 2
> Dikerjakan oleh: Amoes Noland (5027231028)

Soal kedua menyuruh kita untuk membuat sebuah daemon yang berfungsi untuk melakukan manipulasi file sesuai yang diminta oleh soal. Untuk itu, perlu diketahui bahwa diperlukan batas buffer untuk banyak string yang akan dibuat, dan peraturan daemon yang mewajibkan directory path lengkap. Sehingga dipersiapkan global variabel sebagai berikut:

```c
// Define some global variables
#define LIBRARY     "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup"
#define MAX_BUFFER  1024
char dir_name[MAX_BUFFER];
```

Dan terdapat satu baris pada main yang terhubung dengan global variable **dir_name** yang krusial untuk semua fungsi lain:

```c
    // Save current directory to global var
    getcwd(dir_name, sizeof(dir_name));
```

Juga dilakukan inisialisasi daemon yang dilakukan dengan cara:

- Fork pertama berdasarkan PID
- Berikan permission lengkap
- Exit parent
- Fork kedua berdasarkan SID
- Change directory ke dalam root
- Tutup semua input/output standard

```c
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    pid_t sid = setsid();
    if (sid < 0)        exit(EXIT_FAILURE);
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Daemon start
    while (1)
    {
        switch(mode){
            case (0):
                branch_default();
                break;
            case (1):
                branch_mode(1);
                break;
            case (2):
                branch_mode(0);
                break;
        }
        sleep(1000);
    }
```

Tugas pertama pada daemon adalah untuk melakukan download dan unzip sebuah file .zip yang terdapat dalam sebuah Google Drive. Hal ini dilakukan dengan kombinasi `fork()` dan `exec()`.

Berikut adalah fungsi **get_library** untuk mendapatkan library dengan fungsi wget:

```c
// Gets library archive
void get_library(){
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : exec process
        chdir(dir_name);
        char *cmd = "/usr/bin/wget";
        char *arg[] = {"wget", "--no-check-certificate", "--content-disposition",
                      LIBRARY, "-P", dir_name, NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}
```

Berikut adalah fungsi **ext_library** untuk melakukan unzip library:

```c
// Extracts library archive
void ext_library(){
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : exec process
        chdir(dir_name);
        char *cmd = "/usr/bin/unzip";
        char *arg[] = {"unzip", "library.zip", NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}
```

Langkah berikutnya yang diminta adalah untuk melakukan dekripsi nama file ke-7 (6 file pertama dapat dibedakan karena mereka dimulai dengan angka) hingga terakhir menggunakan algoritma ROT-19 (geser huruf). Dasar fungsi yang akan digunakan menerima input char dan melakukan return hasil dekripsi char secara ROT-19.

```c
// ROT19 translator module
char rot19(char input){
   if (!(isalpha(input))) return input;
   char base = islower(input) ? 'a':'A';
   return (input - base + 7) % 26 + base;
}
```

Fungsi ini akan digunakan dalam sebuah loop yang melakukan rename pada semua file di dalam directory library yang memenuhi kriteria:
```c
// Main rename loop
void default_mode(){
    char dir_lib[MAX_BUFFER]; 
    strcpy(dir_lib, dir_name);
    strcat(dir_lib, "/library");
    char buffer[MAX_BUFFER], translate[MAX_BUFFER];

    DIR *dir = opendir(dir_lib);
    struct dirent *ep;
    if (!dir) return;

    chdir(dir_lib);
    while(ep = readdir(dir)){
        if ((strcmp(ep->d_name, ".") == 0 )|| (strcmp(ep->d_name, "..") == 0)) continue;
        strcpy(buffer, ep->d_name);
        // Translate the non-number prefixed files
        if(isalpha(buffer[0])){
            strcpy(translate, buffer);
            for (int i=0; buffer[i] != '\0'; i++){
                buffer[i] =  rot19(translate[i]);
            }
            rename(translate, buffer);
        }
```

Setelah dekripsi selesai, kita harus melihat nama file untuk mencari kode r3N4mE, d3Let3, dan m0V3.

- d3Let3 : hapus file
- r3N4mE : rename file sesuai ketentuan pada soal
- m0V3 : untuk perintah berikutnya (mode)

Segala hasil rename dan delete akan tercatat dalam sebuah log file yang akan nanti dibahas lebih lanjut.

```c
        // Main checks for actions
        if(strstr(buffer, "d3Let3") != NULL){
            remove(buffer);
            log_write(buffer, 2);
        } else if(strstr(buffer, "r3N4mE") != NULL){
            if(strstr(buffer, ".ts") != NULL){
                rename(buffer, "helper.ts");
                log_write(buffer, 1);
            }
       else if(strstr(buffer, ".py") != NULL){ 
                rename(buffer, "calculator.py");
                log_write(buffer, 1);
            }
       else if(strstr(buffer, ".go") != NULL){ 
                rename(buffer, "server.go");
                log_write(buffer, 1);
            } 
            else{
                rename(buffer, "renamed.file");
                log_write(buffer, 1);
            } 
        } else if(strstr(buffer, "m0V3") != NULL){
            continue;
        }
        // sleep(1);
    }
    closedir(dir);
    return;
}
```

Berikut adalah hasil ketika `./management` dijalankan:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232703556595875861/Screenshot_20240424_214037.png?ex=662a6c26&is=66291aa6&hm=dc6c033838bca4d33aeacf08017481ea3412b29f12a16ee7769a0a737c81a62a&=&format=webp&quality=lossless&width=703&height=396)

Untuk langkah berikutnya kita harus membuat mode *backup* dan *restore* yang dapat dipanggil dengan command yang sesuai sehingga diperlukan if conditional yang membaca argument sesuai option `-m backup` atau `-m restore` sehingga terlihat seperti berikut.

```c
    // Select mode from args
    if ((argc == 3)&&(strcmp(argv[1], "-m") == 0)){
        // Check for mode arguments
        if (strcmp(argv[2], "backup") == 0)  mode = 1;
        if (strcmp(argv[2], "restore") == 0) mode = 2;
    }
```

Untuk menentukan cabang mode, dilakukan switch case pada loop utama daemon sebagai berikut:

```c
    // Daemon start
    while (1)
    {
        switch(mode){
            case (0):
                branch_default();
                break;
            case (1):
                branch_mode(1);
                break;
            case (2):
                branch_mode(0);
                break;
        }
        sleep(1000);
    }
```

Dengan pertama fungsi **branch_default** yang memanggil deretan fungsi yang akan hanya dijalankan sekali:

*catatan: reset_default merupakan fungsi yang digunakan untuk testing menghapus file library setiap kali mode default berhasil dijalankan.*

*catatan 2: touch_log akan relevan pada bagian pembuatan history log*

```c
void branch_default(){
    // Obtain library only once
    static int got = 0;
    if (!got)
    {
        reset_default();
        touch_log();
        get_library();
        ext_library();
        default_mode();
        got = 1;
    }
}
```

Karena mode backup dan restore pada intinya hanya dilakukan perpindahan file, maka dipanggil melalui fungsi yang sama namun hanya dengan urutan destinasi yang berbeda. Tetapi, untuk memastikan bahwa fungsi berjalan dengan baik, terdapat beberapa fungsi dependency yang berpengaruh seperti:

**backup_check** : memastikan folder backup ada

```c
// Checks existence of a directory
int backup_check(const char *dir_bak){
    struct stat stats;
    if (stat(dir_bak, &stats) == 0) return 0;
    return 1;
}
```

**backup_init** : membuat folder backup

```c
// Function to make directory
void backup_init(char *dir_bak){
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : make directory
        char *cmd = "/usr/bin/mkdir";
        char *arg[] = {"mkdir", dir_bak, NULL};
        execvp(cmd, arg);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}
```

**backup_move** : fork untuk memindahkan file

```c
// Function to move selected item
void backup_move(char *source, char *dest){
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : make directory
        char *cmd = "/usr/bin/mv";
        char *arg[] = {"mv", source, dest, NULL};
        execvp(cmd, arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}
```

**branch_mode** : fungsi utama dari mode backup dan restore yang dibedakan dengan variabel `int backup`

```c
// Main function for backup and restore
void branch_mode(int backup){
    char dir_lib[MAX_BUFFER]; 
    strcpy(dir_lib, dir_name);
    strcat(dir_lib, "/library/");
    
    char dir_bak[MAX_BUFFER];
    strcpy(dir_bak, dir_lib);
    strcat(dir_bak,"backup/");

    char buffer[MAX_BUFFER],
    buffer_lib[MAX_BUFFER],
    buffer_bak[MAX_BUFFER];

    if (backup_check(dir_bak)) backup_init(dir_bak);

    DIR *dir;
    if (backup) dir = opendir(dir_lib);
    else        dir = opendir(dir_bak);
    struct dirent *ep;
    if (!dir) return;

    while(ep = readdir(dir)){
        strcpy(buffer, ep->d_name);
        if ((strcmp(ep->d_name, ".") == 0 )||
            (strcmp(ep->d_name, "..") == 0)||
            (strstr(buffer, "m0V3") == NULL)) continue;

        strcpy(buffer_lib, dir_lib);
        strcpy(buffer_bak, dir_bak);
        strcat(buffer_lib, buffer);
        strcat(buffer_bak, buffer);

        if (backup) backup_move(buffer_lib, buffer_bak);
        else        backup_move(buffer_bak, buffer_lib);
        log_write(buffer, 4-backup);
        // sleep(1);
    }
    closedir(dir);
    return;
}
```

Berikut adalah hasil ketika `./management` dijalankan dengan mode backup dan restore:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232703556084306002/Screenshot_20240424_214114.png?ex=662a6c26&is=66291aa6&hm=18b36f7c2bcd88cb74de00bd9ea12ed7c0ca6e629a253f4fd64c08cdcdf5435c&=&format=webp&quality=lossless&width=703&height=396)

Fitur berikutnya yang diperlukan adalah untuk memanggil signal, yang dilakukan dengan membuat sebuah variabel atomic yang dinamis sehingga dapat dihubungkan dengan sebuah signal handler:

```c
// Set mode signals
volatile sig_atomic_t mode = 0;
void signal_handler(int sig){
    switch (sig)
    {
    case 34:
        mode = 0;
        break;
    case SIGUSR1:
        mode = 1;
        break;
    case SIGUSR2:
        mode = 2;
        break;
    case SIGTERM:
        exit(EXIT_SUCCESS);
        break;
    }
}
```

Signal handler tersebut dihubungkan pada main agar dapat inisialisasi signal handling sesuai kode signal:

```c
void set_signals(){
    // Attempt to grab signals
    signal(SIGRTMIN, signal_handler);
    signal(SIGUSR1,  signal_handler);
    signal(SIGUSR2,  signal_handler);
    signal(SIGTERM,  signal_handler);
}
```

Berikut adalah hasil ketika perubahan mode dijalankan dengan mengirimkan signal:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232703554884734997/Screenshot_20240424_214212.png?ex=662a6c26&is=66291aa6&hm=23092bc2240531f8600eea7cfd984be2e103764519792602a54e3748d13d0939&=&format=webp&quality=lossless&width=703&height=396)
![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232703554410643517/Screenshot_20240424_214241.png?ex=662a6c25&is=66291aa5&hm=d83e433fe008ac5a57a11f70426e6461b139e988164d816ed8e9a1be0760570f&=&format=webp&quality=lossless&width=703&height=396)

Terakhir, diperlukan sistem mencatat log yang pertama harus dimulai dengan memastikan bahwa file log sudah ada dengan menjalankan fungsi touch yang terdapat pada **touch_log**:

```c
void touch_log(){
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : exec process
        chdir(dir_name);
        char *cmd = "/usr/bin/touch";
        char *arg[] = {"touch", "history.log", NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}
```

Untuk penulisan log sudah terhubung dengan semua kejadian yang ingin dicatat, dan dilakukan melalui sebuah kode aksi yang dihubungkan dengan sebuah switch case.

Penulisan log itu sendiri diperlukan username yang didapat melalui `getlogin_r` dan waktu yang diperoleh dari library `<time.h>`

```c
// Log setup
void log_write(char *name, int act_code){
    char  dir_log[MAX_BUFFER]; 
    strcpy(dir_log, dir_name);
    strcat(dir_log, "/history.log");

    FILE *file;
    file = fopen(dir_log, "a");
    if (!file) return; 
    
    char uname[MAX_BUFFER]; getlogin_r(uname, sizeof(uname));
    char buffer[MAX_BUFFER];
    time_t timevar; struct tm *timeinfo;
    time (&timevar); timeinfo = localtime (&timevar);

    // 1 == rename, 2 == delete, 3 == backup, 4 == restore
    switch (act_code)
    {
    case 1:
        sprintf(buffer, "[%s][%02d:%02d:%02d] - %s - Succesfully renamed.",
        uname, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, name);
        break;
    case 2:
        sprintf(buffer, "[%s][%02d:%02d:%02d] - %s - Succesfully deleted.",
        uname, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, name);
        break;
    case 3:
        sprintf(buffer, "[%s][%02d:%02d:%02d] - %s - Succesfully moved to backup.",
        uname, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, name);
        break;
    case 4:
        sprintf(buffer, "[%s][%02d:%02d:%02d] - %s - Succesfully restored from backup.",
        uname, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, name);
        break;
    }

    fprintf(file, "%s\n", buffer);
    fclose(file);
}
```

## Soal 3
> Dikerjakan oleh: Malvin Putra Rismahardian (5027231048)

### isi soal

Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!

a. Nama program tersebut dengan nama admin.c

b. Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command:
./admin <user>

c. Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 
./admin -m <user>
Dan untuk mematikan fitur tersebut menggunakan: 
./admin -s <user>
Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:
[dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN

d. Program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan: 
./admin -c user
sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:
./admin -a user

e. Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN


### Penyelesaian
**Pertama**

Saya membuat direktori baru dengan nama `admin.c`, lalu mengisi `admin.c` dengan code yang sesuai dengan cara `nano admin.c` dan mulai membuat code.


     #define LOG_FILE_EXTENSION ".log"
 * Command ini adalah definisi konstanta `LOG_FILE_EXTENSION` yang menyimpan ekstensi file log. Dalam kasus ini, ekstensi log adalah `.log`.


**Kedua**

 ```sh
 void write_log(const char *user, const char *activity, bool success) {
    // Deklarasi variabel
    time_t current_time;
    struct tm *time_info;
    char time_str[20];
    char log_filename[50];
    FILE *log_file;

    // Dapatkan waktu saat ini
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%d:%m:%Y-%H:%M:%S", time_info);

    // Buat nama file log sesuai dengan user
    snprintf(log_filename, sizeof(log_filename), "%s%s", user, LOG_FILE_EXTENSION);

    // Buka file log
    log_file = fopen(log_filename, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    // Tulis log ke file
    if (success) {
        fprintf(log_file, "[%s]-pid_kegiatan-%s_JALAN\n", time_str, activity);
    } else {
        fprintf(log_file, "[%s]-pid_kegiatan-%s_GAGAL\n", time_str, activity);
    }

    // Tutup file log
    fclose(log_file);
}
```
* Ini adalah fungsi `write_log` yang digunakan untuk menulis kegiatan pengguna ke dalam file log. Fungsi ini menerima tiga parameter: `user` (nama pengguna), `activity` (kegiatan yang dilakukan), dan `success` (keberhasilan kegiatan). Fungsi ini mencatat waktu saat ini, membuat nama file log sesuai dengan pengguna, membuka file log, menulis kegiatan ke dalam file log, dan menutup file log.

**Ketiga**

```sh
void monitor_user_activity(const char *user) {
    char activity[100];

    // Loop untuk terus memantau kegiatan user
    while (1) {
        // Misalnya, Anda dapat membaca kegiatan user dari input atau sumber lainnya
        // Di sini, untuk contoh, kita menggunakan kegiatan statis
        strcpy(activity, "Example activity");

        // Catat kegiatan user ke dalam log
        write_log(user, activity, true);

        // Tunggu beberapa detik sebelum memantau kegiatan berikutnya
        sleep(1);
    }
}
```
* Ini adalah fungsi `monitor_user_activity` yang digunakan untuk memantau kegiatan pengguna. Fungsi ini menerima satu parameter: `user` (nama pengguna). Fungsi ini memiliki loop yang terus berjalan untuk memantau kegiatan pengguna. Di dalam loop tersebut, kegiatan pengguna (dalam contoh ini, disetel statis sebagai "Example activity") dicatat dalam file log menggunakan fungsi `write_log`.


**Keempat**

```sh
int main(int argc, char *argv[]) {
    // Memeriksa jumlah argumen yang diberikan
    if (argc < 3) {
        printf("Usage: %s <command> <user>\n", argv[0]);
        printf("Commands:\n");
        printf("  -m : Monitor user activity\n");
        printf("  -s : Stop monitoring user activity\n");
        printf("  -c : Cancel user activities\n");
        printf("  -a : Allow user activities\n");
        return EXIT_FAILURE;
    }

    // Mendapatkan command dan user dari argumen
    char *command = argv[1];
    char *user = argv[2];

    // Memeriksa command yang diberikan
    if (strcmp(command, "-m") == 0) {
        // Memantau kegiatan pengguna
        printf("Monitoring user: %s\n", user);
        monitor_user_activity(user);
    } else if (strcmp(command, "-s") == 0) {
        // Menghentikan pemantauan kegiatan pengguna
        printf("Stop monitoring user: %s\n", user);
        // Implementasi stop monitoring user
    } else if (strcmp(command, "-c") == 0) {
        // Membatalkan kegiatan pengguna
        printf("Cancel activities for user: %s\n", user);
        write_log(user, "User sedang aktif", false);
    } else if (strcmp(command, "-a") == 0) {
        // Mengizinkan kegiatan pengguna kembali
        printf("Allow activities for user: %s\n", user);
        write_log(user, "User sedang aktif", true);
    } else {
        // Command tidak dikenali
        printf("Unknown command: %s\n", command);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
* Ini adalah fungsi `main`. Program utama ini menerima argumen dari baris perintah. Jika argumen yang diberikan tidak sesuai, program akan mencetak pesan usage dan keluar dengan kode keluar `EXIT_FAILURE`. Jika argumen sesuai, program akan mengeksekusi perintah yang diberikan (monitor, stop, cancel, atau allow) sesuai dengan argumen, dan kemudian mencetak pesan sesuai dengan tindakan yang dilakukan. Jika perintah tidak dikenali, program akan mencetak pesan bahwa command tidak dikenali dan keluar dengan kode keluar `EXIT_FAILURE`.


### Cara Menjalankan code

Compile file `admin.c` menjadi sebuah program eksekusi dengan menggunakan compiler C, seperti `gcc`. Contohnya:
```sh
gcc admin.c -o admin
```
* Perintah di atas akan menghasilkan file eksekusi bernama `admin`.

Setelah berhasil dikompilasi, program admin dapat dijalankan dengan memberikan argumen sesuai dengan perintah yang diinginkan. Misalnya, untuk memulai memantau kegiatan user dengan cara:
```sh
./admin -m nama_user
```

Untuk menghentikan pemantauan kegiatan user, gunakan:
```sh
./admin -s nama_user
```

Untuk menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan:
```sh
./admin -c nama_user
```

Fitur ini dapat dimatikan dengan command:
```sh
./admin -a nama_user
```

Untuk melihat log bisa dilakukan dengan command:
```sh
cat nama_user.log
```


### Screenshoot Hasil Pengerjaan

Berikut ini adalah contoh menjalankan program dari `admin.c`

![alt text](https://cdn.discordapp.com/attachments/1233746742990868520/1233747016333660240/Screenshot_2024-04-27_184826.png?ex=662e37f2&is=662ce672&hm=f2e7cbc64bd7114061c2648c4a16f8d1bb0dd2bbfb99c798d37458a5ac29f9b8&)

Berikut ini adalah isi didalam `Namauser.log`
![alt text](https://cdn.discordapp.com/attachments/1233746742990868520/1233747015796654090/Screenshot_2024-04-27_184231.png?ex=662e37f2&is=662ce672&hm=2613f98b9a90e9a4ae533fe12d8673b7bd8c2af2556cc675590cb2a1c1683671&)

### kendala
Tidak ada kendala. 

## Soal 4
> Dikerjakan oleh: Amoes Noland (5027231028)

Soal keempat menyuruh kita untuk melakukan setup membuka/menutup aplikasi dengan jumlah lebih dari satu. Untuk itu diperlukan sebuah infrastruktur untuk mencatat jendela aplikasi apa saja yang terbuka agar dapat ditutup dengan memanggil PID yang sesuai.

Fungsi dasar pertama adalah untuk mempersiapkan variabel global dalam proses.

```c
// Global variable preparations
#define PID_MAX  20 //for controlling loop count
#define ARG_SIZE 50 //for controlling str memory allocations
#define TMP_FILE ".temp.txt"
pid_t running[PID_MAX];
int pidcount = 0;
```

Dua fungsi berikutnya adalah untuk menulis/membaca sebuah file sementara yang menyimpan jumlah window dan PID yang terhubung dengan setiap proses. Hal ini dilakukan dengan `fopen` dan melakukan parsing yang sesuai untuk membaca/menyimpan variabel yang sesuai.

```c
void read_temp(){
    // Get file
    FILE *input_file = fopen(TMP_FILE, "r");
    if (0 == input_file){
        printf("Error: Failed to open temp\n");
        return;
    }

    // Read through file
    if (input_file != NULL) {
        // Get PID count
        fscanf(input_file, "%d", &pidcount);
        // Loop through PIDs
        for (int i = 0; i < pidcount; i++) {
            fscanf(input_file, "%d", &running[i]);
        }
        fclose(input_file);
    }
}

void write_temp(){
    // Get file
    FILE *input_file = fopen(TMP_FILE, "w");
    if (0 == input_file){
        printf("Error: Failed to open temp\n");
        return;
    }

    // Write in file
    if (input_file != NULL) {
        // Write PID count
        fprintf(input_file, "%d\n", pidcount);
        // Loop through PIDs to write
        for (int i = 0; i < pidcount; i++) {
            fprintf(input_file, "%d\n", running[i]);
        }
        fclose(input_file);
    }
}
```

Untuk fungsi pertama, kita disuruh untuk membuka berbagai jenis aplikasi dan banyak window aplikasi sekaligus dengan format command:

```
./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
```

Harus dibangun sebuah infrastruktur untuk membaca argumen command sesuai opsi. Ini dilakukan dengan menggunakan switch case option yang terhubung ke berbagai fungsi. Untuk fungsi pertama adalah sebagai berikut:

```c
    // Read input arguments
    // Switch case to select option from 2nd char from argv[1]
    if (argv[1][0] != '-'){
        printf("Exited: Invalid option");
        exit(1);
    }
    switch (argv[1][1])
    {
        case 'o':
            open(argc, argv);
            write_temp();
            break;
```

Fungsi **open** melakukan loop melalui semua argumen pada command yang dikirim sehingga habis, dan argumen yang terkirim dipastikan valid agar dapat dilakukan loop melalui semua jenis window dan jumlah window yang akan dibuka dengan melakukan `fork()` dan `execlp()` agar dapat eksekusi program yang terdapat pada `$PATH` device yang digunakan. Setelah itu, PID dari hasil exec akan disimpan dalam global array untuk dicatat ke temp file.

```c
void open(int argc, char *argv[])
{
    // Check input validity
    for (int i=2; i<argc; i+=2){
        if (!(*argv[i+1]-'0' >= 0 && *argv[i+1]-'0' <= 9)){
            printf("Error: Invalid argument(s)");
            exit(1);
        }
    }
    // Loop to run processes based on arg count
    for (int i=2; i<argc; i+=2){
        for (int j=0; j<*argv[i+1]-'0'; j++){
            pid_t pid = fork();
            if (pid < 0) {
                printf("Error: Fork failed\n");
                exit(1);
            }
            if (0 == pid){
                // Child : exec process
                execlp(argv[i],argv[i],NULL);
                exit(0);
            }
            else {
                // Parent : add PIDs to global array
                add_global(pid);
            }
        }
    }
}
```

Berikut adalah contoh penerapan membuka banyak window sekaligus:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232545567973052486/Screenshot_20240424_111026.png?ex=6629d902&is=66288782&hm=4b4fa02987db382a7281d48ba7911f88bf256d8e0af83ab8115c8c55f6a49516&=&format=webp&quality=lossless&width=703&height=396)

Berikut adalah contoh temp file yang dihasilkan dari membuka 3 window:

```
3
93288
93290
93290
```

Untuk fungsi kedua, kita disuruh untuk membuka berbagai jenis aplikasi dan banyak window aplikasi sekaligus dengan konfigurasi sebuah file dengan format command:

```
./setup -f <file>
```

Switch case yang digunakan adalah sebagai berikut:

```c
    case 'f':
        open_file(argc, argv);
        write_temp();
        break;
```

Fungsi **open_file** melakukan loop dalam file konfigurasi agar dapat dilakukan parse membuka window aplikasi sesuai jumlah yang ditentukan dalam format:

```
<app1> <num1>
<app2> <num2>
...
<appN> <numN>
```

```c
void open_file(int argc, char *argv[]){
    // Get file from argument
    FILE *input_file = fopen(argv[2],"r");
    if (0 == input_file){
        printf("Error: Failed to open file");
        exit(1);
    }
    if (input_file != NULL) {
        char argopen[ARG_SIZE]; int argnum;
        // Loop through conf file
        while (fscanf(input_file, "%s %d", argopen, &argnum) == 2) {
            for (int i=0; i<argnum; i++){
                pid_t pid = fork();
                if (pid < 0) {
                    printf("Error: Fork failed\n");
                    exit(1);
                }
                if (0 == pid){
                    // Child : exec process
                    execlp(argopen,argopen,NULL);
                    exit(0);
                }
                else {
                    // Parent : add PIDs to global array
                    add_global(pid);
                }
            }
        }
        fclose(input_file);
    }
}
```

Berikut adalah contoh penerapan membuka banyak window menggunakan sebuah file konfigurasi:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232545566874144788/Screenshot_20240424_111430.png?ex=6629d902&is=66288782&hm=e86a902073c3b45e51a942d05ebf48f2cfe6f61b4d501176facaa2a8e82bb6b8&=&format=webp&quality=lossless&width=703&height=342)
![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232545566601252884/Screenshot_20240424_111451.png?ex=6629d902&is=66288782&hm=943adc9b5a95262b47714a5cb97beb240c392cd6f3d2f8a928cb4f64646d1e46&=&format=webp&quality=lossless&width=703&height=396)

Untuk fungsi ketiga, kita disuruh untuk mematikan hanya  aplikasi yang sudah dijalankan sebelumnya, atau berdasarkan konfigurasi dengan format command:

```
./setup -k
./setup -k <file>
```

Switch case yang digunakan adalah sebagai berikut:

```c
    case 'k':
        // Branches based on the existence of conf   
        if (3 == argc){
            kill_file(argc, argv);
        }
        else{
            read_temp();
            kill_temp();
        }
        break;
```

Untuk fungsi **kill_temp** dilakukan loop melalui global variable dan array yang sudah dibaca dari temporary file, sehingga dapat dengan mudah melakukan loop untuk kill PID yang sudah tercatat pada temporary file.

```c
void kill_temp(){
    // Use loaded global variables to loop killing PIDs
    for (int i = 0; i < pidcount; i++) {
        // printf("Debug: attempt kill PID  %d\n", (int)running[i]);
        if (kill(running[i], SIGTERM) == -1) {
            perror("Error");
        }
    }
}
```

Berikut adalah contoh hasil akhir penerapan kill window dari panggilan command -o:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232545567192907879/Screenshot_20240424_111051.png?ex=6629d902&is=66288782&hm=eeb239c30334501621dc20a370cf0ef78d7cc20641c94221f240e54273945eeb&=&format=webp&quality=lossless&width=703&height=396)

Untuk fungsi **kill_file** pada dasarnya menggunakan prinsip yang sama dengan fungsi **open_file** untuk cara parsing yang dilakukan, tetapi dengan command execlp yang berbeda, karena bertujuan untuk kill process yang disebutkan pada argumen command yang diberikan pada file konfigurasi.

```c
void kill_file(int argc, char *argv[]){
    // Get file from argument
    FILE *input_file = fopen(argv[2],"r");
    if (0 == input_file){
        printf("Error: Failed to open file");
        exit(1);
    }
    if (input_file != NULL) {
        char argkill[ARG_SIZE]; int argnum;
        // Loop through conf file
        while (fscanf(input_file, "%s %d", argkill, &argnum) == 2) {
            for (int i=0; i<argnum; i++){
                pid_t pid = fork();
                if (pid < 0) {
                    printf("Error: Fork failed\n");
                    exit(1);
                }
                if (0 == pid){
                    // Child : exec process
                    execlp("pkill", "pkill", argkill, NULL);
                    exit(0);
                }
            }
        }
        fclose(input_file);
    }
}
```

Berikut adalah contoh hasil akhir penerapan kill window dari panggilan command -f:

![alt text](https://media.discordapp.net/attachments/1071478813566976151/1232545567192907879/Screenshot_20240424_111051.png?ex=6629d902&is=66288782&hm=eeb239c30334501621dc20a370cf0ef78d7cc20641c94221f240e54273945eeb&=&format=webp&quality=lossless&width=703&height=396)

### Kendala
Terdapat sebuah kendala pada fungsi **kill_file** di mana pkill tidak bisa melakukan kill process sesuai jumlah yang ditentukan karena command membunuh *semua* proses dengan nama yang tercatat pada file konfigurasi.

```c
    if (0 == pid){
        // Child : exec process
        execlp("pkill", "pkill", argkill, NULL);
        exit(0);
    }
```
