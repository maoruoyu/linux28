#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

//void myfunc(int signo)
// {
// 	printf("hello\n");
// }

int main(int argc, char *argv[])
{
	struct itimerval newt;
	struct itimerval oldt;
	
  	//signal(SIGALRM,myfunc);//注册SIGALRM信号的捕捉处理函数
	
	newt.it_interval.tv_sec=0;
	newt.it_interval.tv_usec=0;//设置单次定时器
	newt.it_value.tv_sec=1;
	newt.it_value.tv_usec=0;

   	if(setitimer(ITIMER_REAL,&newt,&oldt)==-1)
	{
		perror("setitimer error");
		return -1;
	}

	//while(1);
	for(int i=0;;i++)
		printf("%d\n",i);
    return 0;
}
