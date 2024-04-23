# Sisop-2-2024-MH-IT29
## Anggota Kelompok:
- Dian Anggraeni Putri (5027231016)
- Amoes Noland (5027231028)
- Malvin Putra Rismahardian (5027231048)


## Soal 1
> Dikerjakan oleh: Dian Anggraeni Putri (5027231016)

## Soal 2
> Dikerjakan oleh: Amoes Noland (5027231028)

## Soal 3
> Dikerjakan oleh: Malvin Putra Rismahardian (5027231048)

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

### Kendala
Terdapat sebuah kendala pada fungsi **kill_file** di mana pkill tidak bisa melakukan kill process sesuai jumlah yang ditentukan karena command membunuh *semua* proses dengan nama yang tercatat pada file konfigurasi.

```c
    if (0 == pid){
        // Child : exec process
        execlp("pkill", "pkill", argkill, NULL);
        exit(0);
    }
```