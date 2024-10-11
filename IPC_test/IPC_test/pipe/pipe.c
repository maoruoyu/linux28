#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>

void sys_err(const char * str)
{
	perror(str);
	exit(1);
	
}

int main(int argc ,char *argv[])
{
	pid_t pid;
	int fd[2];
	int ret;

	char * str="hello pipe\n";
	char buf[1024];

	ret = pipe(fd);
	if(ret==-1)
	{
		sys_err("pipe error");
	}	

	pid=fork();
	if(pid==-1)
	{
		sys_err("fork error");
	}
	if(pid>0)
	{
		close(fd[0]);
		write(fd[1],str,strlen(str));
		close(fd[1]);
	}
	else if(pid == 0)
	{
		close(fd[1]);
		ret=read(fd[0],buf,sizeof(buf));
		write(STDOUT_FILENO,buf,ret);
		close(fd[0]);
	}

	return 0;
}
