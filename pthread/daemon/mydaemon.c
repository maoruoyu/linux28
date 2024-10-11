#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

void sys_err(char * str)
{
	perror(str);
	exit(-1);
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int ret,fd;
	pid=fork();
	if(pid>0)
	{
		exit(0);
	}
	pid=setsid();//创建新会话
	if(pid==-1)
		sys_err("setsid error");
	ret=chdir("/home/mrm/User");//改变工作目录
	if(ret==-1)
		sys_err("chdir error");
	umask(0022);//改变文件访问权限
	close(STDIN_FILENO);//关闭文件描述符0（标准输入）
	fd=open("/dev/null",O_RDWR);//fd=0
	if(fd==-1)
	{
		sys_err("open error");
	}
	dup2(fd,STDOUT_FILENO);
	dup2(fd,STDERR_FILENO);//重定向

	while(1);//模拟守护进程业务逻辑
    
    return 0;
}

