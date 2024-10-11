#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>

int main(int argc,char * argv[])
{
    int i=0;
    pid_t pid,wpid;
    for(i=0;i<3;i++)
    {
        pid=fork();
        if(pid==0)
        {
            break;
        }
    }
    if(i==3)
    {
        
        while((wpid=waitpid(-1,NULL,WNOHANG))!=-1)
        {
            if(wpid>0)
            {
                printf("wait child finish:%d\n",wpid);
            }
            if(wpid==0)
            {
                sleep(1);
                continue;
            }
        }
        printf("parent pid:%d,all child finish\n",getpid());
        return 0;
    }
    switch(i)
    {
        case 0:
            {
            sleep(1);
            execlp("ps","ps","aux",NULL);
                break;
            }
        case 1:
            {
            sleep(2);
            execl("./a","./a",NULL);
                break;
            }
        case 2:
            {
            sleep(3);
            execl("./t","./t",NULL);
                break;
            }
    }
    return 1;
}