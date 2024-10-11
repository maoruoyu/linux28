#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void * tfn1(void * arg)
{
	printf("thread1 running\n");
	return (void*)111;  //exit(111)
}

void * tfn2(void * arg)
{
	printf("thread2 running\n");
	pthread_exit((void*)222);
}

void * tfn3(void * arg)
{
	while(1)
	{
    	// printf("thread3:i am going to die in 3 seconds...\n");
		// sleep(1);
		pthread_testcancel();//自己添加测试取消点  pthread_cancel函数需要契机进入内核才能完成对线程的杀死
	}
    return (void *)666;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
	void * retval=NULL;
	
    pthread_create(&tid,NULL,tfn1,NULL);
	pthread_join(tid,&retval);
	printf("thread 1 exit code %d\n",(int)retval);

    pthread_create(&tid,NULL,tfn2,NULL);
	pthread_join(tid,&retval);
	printf("thread 2 exit code %d\n",(int)retval);

    pthread_create(&tid,NULL,tfn3,NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_join(tid,&retval);
	printf("thread 3 exit code %d\n",(int)retval);//打印的异常退出值-1，retval在前两个线程已经做了修改，不可能是0，异常退出也得不到返回值666，

   	pthread_exit((void*)0);

    //return 0;
}

