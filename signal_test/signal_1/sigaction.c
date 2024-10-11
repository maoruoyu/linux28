#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

void sys_err(char * str)
{
	perror(str);
	exit(1);
}

void sig_catch(int signo)
{
	printf("catch you!!   :%d\n",signo);
	return;
}

int main(int argc, char *argv[])
{
	struct sigaction newaction,oldaction;
	newaction.sa_handler=sig_catch;
	sigemptyset(&newaction.sa_mask);//清空sa_mask信号屏蔽字,只在信号捕捉函数期间生效
	newaction.sa_flags=0;

	int ret=sigaction(SIGINT,&newaction,&oldaction);//注册信号捕捉函数
	if(ret==-1)
	{
		sys_err("sigaction error");
	}
	
	while(1);
    
    return 0;
}

