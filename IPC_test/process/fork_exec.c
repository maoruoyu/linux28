#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if(pid==-1)
    {
        perror("fork error");
        exit(-1);
    }
    else if(pid==0)
    {
		//execlp("ls","-l","-d","-h",NULL);  错误写法
		
		execlp("ls","ls","-l","-d","-h",NULL); 
		//char *argv[]={"ls","-l","-d","-h",NULL};
		//execvp("ls",argv);

		perror("exec error");
		exit(-1);
    }
    else if(pid>0)
    {
		sleep(1);
		printf("i am child process: %d\n",getpid());
    }


    return 0;
}
