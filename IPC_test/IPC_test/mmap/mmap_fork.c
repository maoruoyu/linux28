#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

void sys_err(char * str)
{
	perror(str);
	exit(-1);
}

int var=100;
int main(int argc, char *argv[])
{
	int * p;
	pid_t pid;
	int fd=open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd==-1)
	{
		sys_err("open error");
	}
	ftruncate(fd,4);
	
	p=(int *)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p==MAP_FAILED)
	{
		sys_err("mmap error");
	}

	close(fd);
	
	pid=fork();
	if(pid==-1)
	{
		sys_err("fork error");
	}
	if(pid==0)
	{
		*p=2000;
		var=1000;
		printf("child : *p=%d,  var=%d\n",*p,var);
	}
	else{
		sleep(1);
		printf("parent: *p=%d,  var=%d\n",*p,var);
		wait(NULL);

		int ret=munmap(p,4);
		if(ret==-1)
		{
			sys_err("munmap error");
		}
		
	}
    
    return 0;
}

