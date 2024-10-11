#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

void sys_err(char * str)
{
	perror(str);
	exit(-1);
}

int main(int argc, char *argv[])
{
	int fd,ret;
	int i=0;
	char buf[4096];
   	ret=mkfifo("fifoWR",0664); 
	if(ret==-1)
	{
		sys_err("mkfifo error");
	}
	fd=open("fifoWR",O_WRONLY);
	if(fd==-1)
	{
		sys_err("open error");
	}
	while(1)
	{
		sprintf(buf,"hello2fifoWR:%d\n",i++);
		write(fd,buf,strlen(buf));
		sleep(1);
	}


	close(fd);

    return 0;
}

