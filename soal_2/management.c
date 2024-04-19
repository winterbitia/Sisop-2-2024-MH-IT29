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

/* Soal_2 - VERSION 0.1
Amoes Noland 5027231028
*/

// Define some global variables
#define LIBRARY     "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup"
#define MAX_BUFFER  1024
char dir_name[MAX_BUFFER];

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
        char *arg[] = {"wget", "--no-check-certificate", LIBRARY,
                      "-O", "library.zip", NULL};
        execvp(cmd,arg);
        exit(0);
    }
    else {
        // Parent : wait for child to die
        int status;
        waitpid(pid, &status, 0);
    }
}

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

char rot19(char input){
   if (!(isalpha(input))) return input;
   char base = islower(input) ? 'a':'A';
   return (input - base + 7) % 26 + base;
}

int main(int argc, char *argv[]){
    getcwd(dir_name, sizeof(dir_name));
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

    while (1) {
        // Obtain library only once
        static int got = 0;
        if (!got){
            get_library();
            ext_library();
            got = 1;
        }
        sleep(10);
    }

    return 0;    
}