#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Amoes Noland 5027231028

void open(int argc, char *argv[])
{
    for (int i=2; i<argc; i+=2){
        if (!(*argv[i+1]-'0' >= 0 && *argv[i+1]-'0' <= 9)){
            printf("Error: Invalid argument(s)");
            exit(1);
        }
    }
    // printf("%d\n", count);

    for (int i=2; i<argc; i+=2){
        for (int j=0; j<*argv[i+1]-'0'; j++){
            pid_t pid = fork();
            if (pid < 0) {
                printf("Error: Fork failed\n");
                exit(1);
            }
            if (0 == pid){
                // printf("debug PID: %d for process %s\n", getpid(), argv[i]); exit(0);
                execlp(argv[i],argv[i],NULL);
                // if(execlp(argv[i],argv[i],NULL) == -1){
                //     printf("Error: Exec failed");
                //     exit(1);
                // }
            }
        }
    }
}


int main(int argc, char *argv[])
{ 
    switch (argv[1][1])
    {
    case 'o':
        printf("-o: using regular syntax...\n");
        open(argc, argv);
        break;
    case 'f':
        printf("-f: using config file...\n");     
        break;
    case 'k':
        printf("-k: killing processes...\n");     
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