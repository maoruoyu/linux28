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
	int fd,len;
	char buf[4096];
	fd=open("fifoWR",O_RDONLY);
	if(fd==-1)
	{
		sys_err("open fifo err");
	}
	while(1)
	{
		len=read(fd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,len);
		
		sleep(2);
	}
	
	close(fd);

	

    return 0;
}

