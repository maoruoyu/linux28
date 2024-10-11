#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void sys_err(const char * str)
{
	perror(str);
	exit(1);
}

void * tfn(void * arg)
{
	int i=(int )arg;//强转
	sleep(i);
	printf("i am %dth thread, pid=%d ,tid=%lu\n",i+1,getpid(),pthread_self());
	return NULL;
}

int main(int argc, char *argv[])
{
    int ret,i;
	pthread_t tid;
	for(i=0;i<5;i++)
	{
		ret=pthread_create(&tid,NULL,tfn,(void *)i);//传参采用值传递，借助强转
		if(ret!=0)
		{
			sys_err("pthread_create error");
		}
	}
	sleep(i);
	
	printf("main: i am main thread, pid=%d ,tid=%lu\n",getpid(),pthread_self());
    return 0;
}

