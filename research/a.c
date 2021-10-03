#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int filedes[2];
    if(pipe(filedes) == -1) {
        perror("pipe");
        exit(1);
    }

    char cmdpath[] = "/bin/ls";
    char cmdname[] ="ls";

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if(pid == 0) {
        while((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
        close(filedes[1]);
        close(filedes[0]);
        execl(cmdpath, cmdname, (char*)0);
        perror("execl");
        _exit(1);
    }
    close(filedes[1]);

    char buffer[4096];
    while(1) {
        ssize_t count = read(filedes[0], buffer, sizeof(buffer));
        if(count == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("read");
                exit(1);
            }
        } else if(count == 0) {
            break;
        } else {
             printf("child output: %.*s\n", count, buffer);
        }
    }
    close(filedes[0]);
    wait(0);
}
