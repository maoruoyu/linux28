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
	//tid=pthread_self();
	
    int ret=pthread_create(&tid,NULL,tfn,NULL);
    if(ret!=0)
    {
        perror("pthread_create error");
    }
    printf("main : pid=%d , tid=%lu\n",getpid(),pthread_self());
    sleep(1);
    // printf("main : pid=%d , tid=%lu\n",getpid(),pthread_self());
    // pthread_exit((void*)0);

    return 0;
}

