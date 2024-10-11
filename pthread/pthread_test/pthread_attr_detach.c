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
    printf("thread : pid=%d , tid=%lu\n",getpid(),pthread_self());
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
	
	pthread_attr_t tattr;
	int ret=pthread_attr_init(&tattr);
	if(ret!=0)
	{
		fprintf(stderr,"pthread_attr_init error:%s\n",strerror(ret));
			exit(1);
	}
	ret=pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);//设置线程属性为分离
	if(ret!=0)
	{
		fprintf(stderr,"pthread_attr_setdetachstate error:%s\n",strerror(ret));
			exit(1);
	}


    ret=pthread_create(&tid,&tattr,tfn,NULL);
    if(ret!=0)
    {
		fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
		exit(1);
    }
	ret=pthread_attr_destroy(&tattr);
	if(ret!=0)
	{
		fprintf(stderr,"pthread_attr_destroy error:%s\n",strerror(ret));
			exit(1);
	}
	ret=pthread_join(tid,NULL);
	if(ret!=0)
	{
		fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
			exit(1);
	}
	
    printf("main : pid=%d , tid=%lu\n",getpid(),pthread_self());
    pthread_exit((void*)0);

}

