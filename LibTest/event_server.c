#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <event2/event.h>
#include <fcntl.h>

void cb_func(evutil_socket_t fd, short what, void* arg)
{
	char buf[256] = {0};
	struct event_base *base = (struct event_base*) arg;

	printf("fd = %d, event = %d", fd, what);

	int len = read(fd, buf, sizeof(buf));

	if(len<=0)
		return;
	buf[len] = '\0';
	printf("read buf = [%s]\n", buf);

	FILE* fp = fopen("serverce_client.txt","a");
	event_base_dump_events(base, fp);
	fclose(fp);

}

int main(int argc, char* argv[])
{
	/* 1. 创建base对象 struct event_base *base = event_base_new()；
	 * 2. 创建/初始化事件
	 *	  调用结束时，事件处于未决状态 
	 *			struct event *ev = event_new(XXXXX);
	 * 3. 将事件插入base  event_add(ev,xxx);
	 * 4. 监听绑定在base上的事件 event_base_dispatch(base); */

	struct event *ev1;
	struct event_base *base = event_base_new();   // 创建base对象

	const char* fifo = "event.fifo";
	unlink(fifo);

	mkfifo(fifo, 0644);   // 创建有名管道进行通信

	int fd = open(fifo, O_RDONLY | O_NONBLOCK, 0644);

	// 监听fd1是否可读， 并设置超时
	ev1 = event_new(base, fd, EV_READ | EV_PERSIST, cb_func, base);

	// 将事件插入base
	event_add(ev1, NULL);

	// 循环等待
	// 监听base上绑定的事件，满足后，回调对应函数
	event_base_dispatch(base);

	// 释放资源
	event_free(ev1);
	event_base_free(base);

	return 0;
}
