#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

typedef struct node
{
    int data;
    struct node *next;
}Node;

Node* head = NULL;

void *produce(void *arg)
{
    // 生产
    while(1)
    {
	// 头插
	Node* pnew = (Node* ) malloc(sizeof(Node));
	pnew->data = rand() % 1000;  // 0-999

	// 使用互斥锁保护共享数据
	pthread_mutex_lock(&mutex);
	pnew->next = head;
	head = pnew;

	printf("====== produce : %ld, %d\n", pthread_self(), pnew->data);
	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&cond);

	sleep(rand() % 3);
    }
    return NULL;
}

void *consume(void *arg)
{
    // 消费
    while(1)
    {
	pthread_mutex_lock(&mutex);
	if(head == NULL)
	{
	    // 线程阻塞
	    pthread_cond_wait(&cond, &mutex);
	}
	Node* pdel = head;
	head = head->next;
	printf(" -------- consume: %ld, %d\n", pthread_self(), pdel->data);
	free(pdel);
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    // 锁初始化
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // 创建生产者和消费者线程
    pthread_t pid1, pid2;

    pthread_create(&pid1, NULL, produce, NULL);
    pthread_create(&pid2, NULL, consume, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);   
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
