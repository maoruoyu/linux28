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

	//char * str="hello pipe\n";
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
        //父进程exec调用ls，重定向标准输出到管道写端，将ls的输出写入管道

        ret=dup2(fd[1],STDOUT_FILENO);
        if(ret==-1)
        {
            sys_err("dup2 error");
        }
        close(fd[0]);
        execlp("ls","ls",NULL);
        sys_err("execlp ls error");


	}
	else if(pid == 0)
	{
        //子进程exec调用wc -l，重定向标准输入到管道读端，从管道读取ls的输出

        ret=dup2(fd[0],STDIN_FILENO);
        if(ret==-1)
        {
            sys_err("dup2 error");
        }
        close(fd[1]);
        execlp("wc","wc","-l",NULL);
        sys_err("execlp wc error");
		//子进程使用管道读取ls输出，会阻塞等待写端写数据，直到父进程写入数据，可以将父子进程的代码互换，父进程先写，子进程先读，就不会出现阻塞


	}

	return 0;
}
