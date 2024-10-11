#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;

void *tfn(void *arg)
{
    srand(time(NULL));

    while (1) {
        pthread_mutex_lock(&mutex);//加锁  可以想象成 锁--（1-- -->0）
        printf("hello ");
        sleep(rand() % 3);	/*模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误*/
        printf("world\n");
        pthread_mutex_unlock(&mutex);//解锁 可以想象成 锁++（0++ -->1）
        sleep(rand() % 3);
        
    }

    return NULL;
}

int main(void)
{
    pthread_t tid;
    srand(time(NULL));

    pthread_mutex_init(&mutex, NULL);//初始化锁 可以认为锁的值为1

    pthread_create(&tid, NULL, tfn, NULL);
    while (1) {

        pthread_mutex_lock(&mutex);
        printf("HELLO ");
        sleep(rand() % 3);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
        

    }
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}




/*线程之间共享资源stdout*/
