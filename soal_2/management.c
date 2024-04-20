#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <libgen.h>
#include <dirent.h>

/* Soal_2 - VERSION 0.5
Amoes Noland 5027231028
*/

// Define some global variables
#define LIBRARY     "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup"
#define MAX_BUFFER  1024
char dir_name[MAX_BUFFER];
static int got = 0;

// Set mode signals
volatile sig_atomic_t mode = 0;
void signal_default(int sig) {
    mode = 0;
}
void signal_backup(int sig) {
    mode = 1;
}
void signal_restore(int sig) {
    mode = 2;
}
void signal_exit(int sig) {
    exit(EXIT_SUCCESS);
}

// Deletes all library starting files
void reset_default(){
    char dir_zip[MAX_BUFFER]; 
    strcpy(dir_zip, dir_name);
    strcat(dir_zip, "/library.zip");

    char dir_lib[MAX_BUFFER]; 
    strcpy(dir_lib, dir_name);
    strcat(dir_lib, "/library/");

    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : remove library.zip
        chdir(dir_name);
        char *cmd = "/usr/bin/rm";
        char *arg[] = {"rm", dir_zip, NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);

    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed\n");
        exit(1);
    }
    if (0 == pid){
        // Child : remove library dir
        chdir(dir_name);
        char *cmd = "/usr/bin/rm";
        char *arg[] = {"rm", "-rf", dir_lib, NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
    }
}

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

// ROT19 translator module
char rot19(char input){
   if (!(isalpha(input))) return input;
   char base = islower(input) ? 'a':'A';
   return (input - base + 7) % 26 + base;
}

// Main rename loop
void default_mode(){
    char *dir_lib; 
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
        // Main checks for actions
        if(strstr(buffer, "d3Let3") != NULL){
            remove(buffer);
        } else if(strstr(buffer, "r3N4mE") != NULL){
            if(strstr(buffer, ".ts") != NULL){
                rename(buffer, "helper.ts");
            } 
            else if(strstr(buffer, ".py") != NULL){
                rename(buffer, "calculator.py");
            } 
            else if(strstr(buffer, ".go") != NULL){
                rename(buffer, "server.go");
            } 
            else{
                rename(buffer, "renamed.file");
            } 
        } else if(strstr(buffer, "m0V3") != NULL){
            continue;
        }
        // sleep(1);
    }
    closedir(dir);
    raise(SIGUSR1);
    return;
}

// Checks existence of a directory
int backup_check(const char *dir_bak){
    struct stat stats;
    if (stat(dir_bak, &stats) == 0) return 0;
    return 1;
}

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

    // printf("%s\n", dir_lib);
    // printf("%s\n", dir_bak);
    // printf("%d\n", backup_check(dir_bak));

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

        // printf("%s\n", buffer);
        // printf("%s\n", buffer_lib);
        // printf("%s\n", buffer_bak);

        if (backup) backup_move(buffer_lib, buffer_bak);
        else        backup_move(buffer_bak, buffer_lib);
        // sleep(1);
    }
    closedir(dir);
    return;
}

void branch_default(){
    // Obtain library only once
    if (!got)
    {
        reset_default();
        get_library();
        ext_library();
        default_mode();
        got = 1;
    }
}

int main(int argc, char *argv[]){
    // Save current directory to global var
    getcwd(dir_name, sizeof(dir_name));

    // Attempt to grab signals
    signal(SIGRTMIN, signal_default);
    signal(SIGUSR1, signal_backup);
    signal(SIGUSR2, signal_restore);
    signal(SIGTERM, signal_exit);
    
    // Select mode from args
    if (argc == 1) mode = 0;
    else if ((argc == 3)&&(strcmp(argv[1], "-m") == 0)){
        // Check for mode arguments
        if (strcmp(argv[2], "backup") == 0)  mode = 1;
        if (strcmp(argv[2], "restore") == 0) mode = 2;
    } else return 0;

    // printf("%s", dir_name);
    // get_library();
    // ext_library();

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
            case 0:
                branch_default();
                break;
            case 1:
                branch_mode(1);
                break;
            case 2:
                branch_mode(0);
                break;
        }
        sleep(10);
    }

    return 0;    
}