#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

pthread_rwlock_t rwlock;
int number = 0;

void *myfunc1(void *arg)
{
    while(1)
    {
	pthread_rwlock_wrlock(&rwlock);
	number++;
	printf(" == write: %ld, %d\n", pthread_self(), number);
	pthread_rwlock_unlock(&rwlock);
	usleep(20);
    }
    return NULL;
}

void *myfunc2(void *arg)
{
    
    while(1)
    {
	pthread_rwlock_rdlock(&rwlock);
	printf(" == read: %ld, %d\n", pthread_self(), number);
	pthread_rwlock_unlock(&rwlock);
	usleep(20);
    }
    return NULL;
}

int main()
{
    pthread_rwlock_init(&rwlock, NULL);

    // 创建线程ID
    pthread_t pid[8];

    // 创建3个写进程
    for(int i = 0; i<3; i++)
    {
	pthread_create(&pid[i], NULL, myfunc1, NULL);
    }

    // 创建5个写进程
    for(int i = 3; i<8; i++)
    {
	pthread_create(&pid[i], NULL, myfunc2, NULL);
    }

    // 回收线程
    for(int i = 0; i<8; i++)
    {
	pthread_join(pid[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
