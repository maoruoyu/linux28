#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex,mutex1;

void * tfn(void * arg)
{
	int tmp;
	printf("child thread will lock mutex1\n");
	tmp=pthread_mutex_lock(&mutex1);
	if(tmp!=0)
	{
		fprintf(stderr,"child thread lock mutex1 failed:%s \n",strerror(tmp));
		return NULL;
	}
	printf("child thread locked mutex1\n");

	sleep(1);

	printf("child thread will lock mutex\n");
	tmp=pthread_mutex_lock(&mutex);
	if(tmp!=0)
	{
		fprintf(stderr,"child thread lock mutex failed:%s \n",strerror(tmp));
		return NULL;
	}
	printf("child thread locked mutex\n");


	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex1);

	return NULL;

}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret=pthread_mutex_init(&mutex,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"pmutex init error:%s\n",strerror(ret));
		exit(1);
	}
	ret=pthread_mutex_init(&mutex1,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"pmutex1 init error:%s\n",strerror(ret));
		pthread_mutex_destroy(&mutex);
		exit(1);
	}
	
	ret=pthread_create(&tid,NULL,tfn,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"pthread create error:%s\n",strerror(ret));
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&mutex1);
		exit(1);
	}
	printf("main thread will lock mutex\n");
	ret=pthread_mutex_lock(&mutex);
	if(ret!=0)
	{
		fprintf(stderr,"main thread lock mutex failed:%s\n",strerror(ret));
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex1);
	pthread_join(tid,NULL);
	exit(1);
	}
	printf("main thread locked muteax\n");

	sleep(1);

	printf("main thread will lock mutex1\n");
	
	ret=pthread_mutex_lock(&mutex1);
	if(ret!=0)
	{
		fprintf(stderr,"main thread lock mutex1 failed:%s\n",strerror(ret));
		exit(1);
	}
	printf("main thread locked mutex1\n");


	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex1);

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex1);

	pthread_join(tid,NULL);

    return 0;
}

