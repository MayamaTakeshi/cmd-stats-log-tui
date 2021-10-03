#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// P pipe
#define P_CHILD_READ    ppipe[0]
#define P_PARENT_WRITE  ppipe[1]

// Q pipe
#define Q_PARENT_READ   qpipe[0]
#define Q_CHILD_WRITE   qpipe[1]

int main(int argc, char** argv) {

    int ppipe[2] = {-1, -1};    // parent -> child 
    int qpipe[2] = {-1, -1};    // child -> parent 
    pid_t childpid = fork();    // fork child
    int loop = 1;               // while loop control
    char cmd[50];               // user command sent through pipe
    char msg[50];               // message read from pipe
    int buf = strlen(cmd) + 1;  // message buffer
    int bytesRead;              // read bytes

    // create pipes p and q
    if (pipe(ppipe) < 0 || pipe(qpipe) < 0)
        // cannot create pipe
        printf("FATAL error: could not create one or both pipes!\n");

    switch (childpid) {
        case -1:
            printf("Fork error");
            break;

        case 0: // child process 
            close(P_PARENT_WRITE); // parent ----> child
            close(Q_PARENT_READ);  // parent <---- child
            
            while (loop) {

                bytesRead = read(P_CHILD_READ, &msg, buf);
                if (bytesRead > 0) {
                    printf("[C] Command received: %s", msg);
                }
                //close(P_CHILD_READ);
            }
            break;

        default: // parent process
            close(P_CHILD_READ);  // parent ----> child
            close(Q_CHILD_WRITE); // parent <---- child

            while (loop) {
                printf("[P] Please enter a command: ");
                scanf("%s", cmd);
                printf("[P] You entered: %s", cmd);

                if (!strcmp(cmd, "exit")) loop = 0; // exit command


                write(P_PARENT_WRITE, cmd, buf);
                close(P_PARENT_WRITE);
                
            }
            break;
    }
    return (EXIT_SUCCESS);
}


