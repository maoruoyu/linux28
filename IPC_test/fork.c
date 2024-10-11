#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    printf("before fork-1-\n");
    printf("before fork-2-\n");
    printf("before fork-3-\n");
    printf("before fork-4-\n");

    pid_t pid = fork();
    if(pid==-1)
    {
        perror("fork error");
        exit(-1);
    }
    else if(pid==0)
    {
        printf("child is created..,my pid: %d, my parent pid:%d\n",getpid(),getppid());
    }
    else if(pid>0)
    {
        printf("parent process: my child is %d, my pid: %d, my parent pid:%d\n", pid,getpid(),getppid());
    }

    pid =getpid();
    printf("pid: %d\n",pid);

    printf("------------------end of file \n");

    return 0;
}
