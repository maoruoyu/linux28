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

void printset(sigset_t * set)
{
	for(int i=1;i<32;i++)
	{
		if(sigismember(set,i))
		printf("%d\t",i);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
    sigset_t set,oldset,pendset;
	int ret;
	ret=sigaddset(&set,SIGINT);
	if(ret==-1)
	{
		sys_err("sigaddset error");
	}
	ret=sigprocmask(SIG_BLOCK,&set,&oldset);
	if(ret==-1)
	{
		sys_err("sigprocmask error");
	}
	
	while(1)
	{
		ret=sigpending(&pendset);
		if(ret==-1)
		{
			sys_err("sigpending error");
		}

		printset(&pendset);
		sleep(1);
	}

    return 0;
}

