#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
//加入waitpid函数，可以指定回收的子进程pid，并可以设置阻塞或非阻塞

int main(int argc, char *argv[])
{
    int i=0;
	pid_t pid,wpid,tmpid;
    for(i=0;i<5;i++)
    {
        if((pid=fork())==0)
        {
			
            break;
        }
        if(i==2)
		{
            //pid=getpid();
            tmpid=pid;
        }
    }
    if(i==5)
    {
       //sleep(5);
	   //wpid=waitpid(-1,NULL,WNOHANG);//回收任意子进程，没有结束的子进程，父进程直接返回0
	   //wpid=waitpid(tmpid,NULL,WNOHANG);//回收指定子进程pid为tmpid的子进程
       wpid=waitpid(tmpid,NULL,0);//回收指定子进程pid为tmpid的子进程，并阻塞
	   if(wpid==-1)
	   {
		   perror("waitpid error");
		   exit(-1);
	   }
	   
        printf("i am parent process,wait a child finish : %d \n",wpid);
    }
    else{
        sleep(i);
        printf("i am %dth child process,pid=%d\n",i+1,getpid());
    }

    //printf("------------------end of file \n");

    return 0;
}
