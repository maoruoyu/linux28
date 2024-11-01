#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void * tfn(void * arg)
{
	while(1)
	{
    	printf("thread: pid=%d , tid=%lu\n",getpid(),pthread_self());
		sleep(1);
	}
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
	//tid=pthread_self();
	
    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0)
    {
		fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
		exit(1);
    }
    printf("main : pid=%d , tid=%lu\n",getpid(),pthread_self());
    //sleep(1);
    // printf("main : pid=%d , tid=%lu\n",getpid(),pthread_self());

	ret=pthread_cancel(tid);
    if(ret!=0)
    {
		fprintf(stderr,"pthread_cancel error:%s\n",strerror(ret));
		exit(1);
	}
	while(1);

   	pthread_exit((void*)0);

    //return 0;
}

