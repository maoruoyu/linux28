#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct msg{
	int num;
	struct msg * next;
};

struct msg * head;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_product=PTHREAD_COND_INITIALIZER;

void err_thread(int ret, char *str)
{
	fprintf(stderr,"%s:%s\n",str,strerror(ret));
	pthread_exit(NULL);
}

void * consumer(void * arg)
{
	struct msg *mp;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		//if(head==NULL)
		while(head==NULL)//使用while循环，防止有多个消费者情况其中一个消费者将数据消费、而被互斥锁阻塞但被条件变量唤醒的消费者线程应再次检查条件是否满足
		{
			pthread_cond_wait(&has_product,&mutex);
		}
		mp=head;
		head=mp->next;
		pthread_mutex_unlock(&mutex);

		printf("--consumer --%d\n",mp->num);
		free(mp);
		sleep(rand()%3);
	}

	return NULL;
}

void * producer(void * arg)
{
	struct msg * mp;
	while(1)
	{
		mp=malloc(sizeof(struct msg));
		mp->num=rand()%1000+1;
		printf("--producer --%d\n",mp->num);

		pthread_mutex_lock(&mutex);
		mp->next=head;
		head=mp;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&has_product);
		sleep(rand()%3);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret;
	pthread_t pid,cid;
	srand(time(NULL));

	ret=pthread_create(&pid,NULL,producer,NULL);//生产者
	if(ret!=0)
		err_thread(ret,"pthread_create producer error");

	ret=pthread_create(&cid,NULL,consumer,NULL);//消费者
	if(ret!=0)
		err_thread(ret,"pthread_create consumer error");

	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	return 0;
}

