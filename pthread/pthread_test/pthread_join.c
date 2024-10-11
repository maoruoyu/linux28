#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct thrd{
	int var;
	char str[256];
};

void sys_err(const char * str)
{
	perror(str);
	exit(1);
}

//常规写法
/*void * tfn(void * arg)
{
	struct thrd *tval;
	//tval=malloc(sizeof(tval));
	tval=malloc(sizeof(struct thrd));
	tval->var=220;
	strcpy(tval->str,"hello thread");

	return (void *)tval;
}*/                                   

/*void * tfn(void * arg)
{
	struct thrd tval;

	tval.var=220;
	strcpy(tval.str,"hello thread");

	return (void *)&tval;//错误，不可返回局部变量地址
}*/

void * tfn(void * arg)
{
	struct thrd *tval=(struct  thrd *)arg;

	tval->var=220;
	strcpy(tval->str,"hello thread");

	return (void *)tval;
}


int main(int argc, char *argv[])
{
	pthread_t tid;
	struct thrd starg;
	struct thrd * retval;
	//int ret=pthread_create(&tid,NULL,tfn,NULL);
    int ret=pthread_create(&tid,NULL,tfn,(void *)&starg);
	if(ret!=0)
	{
		sys_err("pthread_create error");
	}
	ret=pthread_join(tid,(void **)&retval);
	if(ret!=0)
	{
		sys_err("pthread_join error");
	}
	printf("child thread exit with var=%d, str=%s\n",retval->var,retval->str);
	pthread_exit(NULL);
	

    return 0;
}

