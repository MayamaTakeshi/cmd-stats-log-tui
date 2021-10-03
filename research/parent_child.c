#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <memory.h>

int main() {
    //for parent to child
    int parentToChild[2];

    //for child to parent
    int childToParent[2];

    //string data
    char message1[] = "massage from child to parent\n";
    char message2[] = "message from parent to child\n";
    char readBuffer[120];
    pipe(parentToChild);
    pipe(childToParent);
    int processId , bytes;

    if((processId = fork())== -1){
        printf("error while creating a child process");
    }

    if(processId == 0){
        //child process here
        //send message from child to parent
        close(childToParent[0]);
        write(childToParent[1] , message1 , strlen(message1)+1);

        //recive message from parent
        close(parentToChild[1]);
        bytes = read(parentToChild[0] , readBuffer , sizeof(readBuffer));
        printf("%s",readBuffer);

        // exit(1);
    }

    if(processId !=0 ){
        //parent process here
        //send message from parent to child
        close(parentToChild[0]);
        write(parentToChild[1] , message2 , strlen(message2)+1);

        //recieve message from child
        close(childToParent[1]);
        bytes = read(childToParent[0] , readBuffer , sizeof(readBuffer));
        printf("%s", readBuffer);
        // exit(1);
    }

    return 0;
}
