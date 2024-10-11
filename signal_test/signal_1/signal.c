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
	signal(SIGINT,sig_catch);

	//
	//
	//
	//
	
	while(1);
    
    return 0;
}

