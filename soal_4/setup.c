#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

/* Soal_4 - VERSION 1
Amoes Noland 5027231028
*/

// Global variable preparations
#define PID_MAX  20 //for controlling loop count
#define ARG_SIZE 50 //for controlling str memory allocations
#define TMP_FILE ".temp.txt"
pid_t running[PID_MAX];
int pidcount = 0;

// Add PIDs to a global array
void add_global(pid_t pid){
    if (pidcount < PID_MAX){
        running[pidcount] = pid;
        pidcount++;
        // printf("Debug: pidcount=%d\n", pidcount);
    }
}

void read_temp(){
    // printf("Debug: read temp\n");     
    
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
    // printf("Debug: write temp\n");     
    
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

void kill_temp(){
    // Use loaded global variables to loop killing PIDs
    for (int i = 0; i < pidcount; i++) {
        // printf("Debug: attempt kill PID  %d\n", (int)running[i]);
        if (kill(running[i], SIGTERM) == -1) {
            perror("Error");
        }
    }
}

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
                // printf("Debug process %s\n", argv[i]);
                add_global(pid);
            }
        }
    }
}

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
                    // printf("Debug process %s\n", argv[i]);
                    add_global(pid);
                }
            }
        }
        fclose(input_file);
    }
}

int main(int argc, char *argv[])
{  
    // Read input arguments
    // Switch case to select option from 2nd char from argv[1]
    if (argv[1][0] != '-'){
        printf("Exited: Invalid option");
        exit(1);
    }
    switch (argv[1][1])
    {
        case 'o':
            // printf("Debug: -o syntax used\n");
            open(argc, argv);
            write_temp();
            break;
        case 'f':
            // printf("Debug: -f syntax used\n");     
            open_file(argc, argv);
            write_temp();
            break;
        case 'k':
            // printf("Debug: -k syntax used\n");  
            // Branches based on the existence of conf   
            if (3 == argc){
                // printf("Debug: conf used\n");     
                kill_file(argc, argv);
            }
            else{
                // printf("Debug: no conf used\n");     
                read_temp();
                kill_temp();
            }
            break;
        default:
            printf("Exited: Invalid option");
            exit(1);
            break;
    }
    
    // for (int i=1; argv[i]!=NULL; i++)
    // printf("Debug: argv[%d]=%s\n",i,argv[i]);
    return 0;
}