#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX 1000

int number = 0;

// 互斥锁
pthread_mutex_t mutex;

void *myfunc1(void *arg)
{
    for(int i = 0; i<MAX ; i++)
    {
	pthread_mutex_lock(&mutex);
	number++;
	printf("Thread 01 , id = %ld, number = %d\n",pthread_self(),number);
	pthread_mutex_unlock(&mutex);
	usleep(20);
    }
    return NULL;
}

void *myfunc2(void *arg)
{
    for(int i = 0; i<MAX ; i++)
    {
	pthread_mutex_lock(&mutex);
	number++;
	printf("Thread 02 , id = %ld, number = %d\n",pthread_self(),number);
	pthread_mutex_unlock(&mutex);
	usleep(20);
    }
    return NULL;
}

int main()
{
    // 创建一个子线程
    // 线程的ID存储
    pthread_t pthid1;
    pthread_t pthid2;

    // 初始化锁
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&pthid1, NULL, myfunc1, NULL);
    pthread_create(&pthid2, NULL, myfunc2, NULL);
    
    pthread_join(pthid1, NULL);
    pthread_join(pthid2, NULL);

    // 销毁锁
    pthread_mutex_destroy(&mutex);
    return 0;
}
