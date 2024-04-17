#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Amoes Noland 5027231028

void run(char process[]){
   printf("\tPID: %d\n\tPPID: %d\nProcess: %s\n", getpid(), getppid(), process);
}

void open(int argc, char *argv[])
{
    int count=0, checked=0;
    for (int i=2; i<argc; i+=2){
        if (!(*argv[i+1]-'0' >= 0 && *argv[i+1]-'0' <= 9)){
            printf("Error: Invalid argument(s)");
            exit(1);
        } count+=(*argv[i+1]-'0');
    } pid_t ids[count];

    // printf("%d\n", count);

    char *process[count];
    for (int i=2; i<argc; i+=2){
        for (int j=checked; j<(*argv[i+1]-'0'+checked); j++){
            process[j] = argv[i];
        } checked+=(*argv[i+1]-'0');
    }

    for (int i=0; i<count; i++){
        ids[i] = fork();
        if (ids[i] < 0) {
            printf("Error: Fork Failed\n");
            exit(1);
        }
        if (0 == ids[i]){
            run(process[i]);
        }
        // printf("%s\n", process[i]);
    } 
}


int main(int argc, char *argv[])
{ 
    switch (argv[1][1])
    {
    case 'o':
        printf("regular command\n");
        open(argc, argv);
        break;
    case 'f':
        printf("conf command\n");     
        break;
    case 'k':
        printf("kill command\n");     
        break;
    default:
        printf("Exited: Invalid option");
        exit(1);
        break;
    }
    
    //testing
    // for (int i=1; argv[i]!=NULL; i++) printf("%s\n",argv[i]);
    return 0;
}