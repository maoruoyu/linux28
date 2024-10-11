#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
// 优化写法见IPC_test/numfork.c


int main()
{
    int i=0;
    for(i=0;i<5;i++)
    {
        pid_t pid=fork();
        if(pid==0)
        {
            sleep(i);
            printf("i'm %dth child process\n",i+1);
            break;
        }
    }
    if(i==5)
    {
        sleep(5);
        printf("i am parent process\n");
        
    }

    // printf("------------------end of file \n");
    
    return 0;
}