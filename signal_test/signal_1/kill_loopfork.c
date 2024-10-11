#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

void sys_err(char * str)
{
	perror(str);
	exit(-1);
}

int main(int argc, char *argv[])
{
	pid_t pid,tmp;
	int ret,i;
	for(i=0;i<5;i++)
	{

		pid=fork();

		if(pid==-1)
		{
			sys_err("fork error");
		}
		if(pid==0)
		{
			while(i==3)
			{
				printf("i am %dth child process,my pid:%d,ppid:%d\n",i,getpid(),getppid());
				sleep(i);
				
			}
			printf("i am %dth child process,my pid:%d,ppid:%d\n",i,getpid(),getppid());
			sleep(i);
			break;
		}
		else
		{
			if(i==3)
				tmp=pid;
		}
	}
	if(i==5)
	{
		printf("i am parent process,  pid: %d\n",getpid());
		sleep(10);
		ret=kill(tmp,SIGKILL);
		if(ret==-1)
		{
			sys_err("kill error");
		}
		
	}

	return 0;
}

