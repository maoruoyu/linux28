#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void sys_err(char * str)
{
	perror(str);
	exit(1);
}

void catch_child(int signo)
{
	pid_t wpid;
	int wstatus;
	//捕捉函数执行期间，本信号自动被屏蔽(sa_flgs = 0)
	while((wpid=waitpid(-1,&wstatus,0))!=-1)
	{  
		 //循环回收子进程，防止产生僵尸进程（避免使用捕获函数执行期间单次回收存在屏蔽SIGCHLD信号问题，多个子进程同时结束时会产生僵尸进程）
		if(WIFEXITED(wstatus))
		{
			printf("catch child & wait 、、、finsh  --- -pid=   :%d   -retnval=   :%d\n",wpid,WEXITSTATUS(wstatus));
			
		}
	}
	return;
}
//注意：SIGCHLD信号默认执行动作为忽略
int main(int argc, char *argv[])
{
	pid_t pid;
	int i,ret;
	//阻塞SIGCHLD防止父进程的SIGCHLD捕获函数还未注册、子进程已经退出（sigprocmask）
	sigset_t set;
	sigemptyset(&set);
	ret=sigaddset(&set,SIGCHLD);
	if(ret==-1)
	{
		sys_err("sigaddset error");
	}
	ret=sigprocmask(SIG_BLOCK,&set,NULL);
	if(ret==-1)
	{
		sys_err("sigprocmask error");
	}
	//sigpending(&pendset);
	for(i=0;i<5;i++)//循环创建多个子进程
	{
		pid=fork();
		if(pid==0)
		{
			//sleep(i);

			break;
		}
	}
	if(i==5)
	{

		printf("i am parent ,pid=%d\n",getpid());

		struct sigaction newaction;
		newaction.sa_handler=catch_child;//设置信号捕捉回调
		sigemptyset(&newaction.sa_mask);//清空sa_mask信号屏蔽字,只在信号捕捉函数期间生效
		newaction.sa_flags=0;//设置默认属性，本信号自动屏蔽

		int ret=sigaction(SIGCHLD,&newaction,NULL);//注册信号捕捉函数
		if(ret==-1)
		{
			sys_err("sigaction error");
		}
		//解除SIGCHLD信号阻塞（sigprocmask）
		sigprocmask(SIG_UNBLOCK,&set,NULL);
		while(1);//模拟父进程后续逻辑

	}
	else
	{
		printf("i am child ,pid=%d\n",getpid());
		return i;
	}

	return 0;
}

