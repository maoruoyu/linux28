#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char * argv[])
{
	pid_t pid,wpid;
	int wstatus;
	pid=fork();
	if(pid==0)
		{
			printf("i am the child going to die,my pid is :%d\n",getpid());
			sleep(10);
			printf("----- child die -----\n");
			return 73;
		}
	else if(pid>0)
		{
			wpid=wait(&wstatus);
			if(wpid==-1)
			{
				perror("wait error");
				exit(-2);
			}
			if(WIFEXITED(wstatus))
			{
				printf("child exit with %d\n",WEXITSTATUS(wstatus));
			
			}
			if(WIFSIGNALED(wstatus))
			{
				printf("child kilded by %d\n",WTERMSIG(wstatus));
			}
			printf("parent wait finish:%d\n",wpid);

		}
	else
		{
			perror("fork error");
			return 1;
		}
	return 0;
}
