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
	const char *data = arg;
	printf("Got an event on socket %d:%s%s%s%s [%s]\n",
			(int) fd,
			(what&EV_TIMEOUT)? "timeout":"",
			(what&EV_READ)? " read":"",
			(what&EV_WRITE)? " write":"",
			(what&EV_SIGNAL)? " signal":"",
			data);
}

void main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
	/* 1. 创建base对象 struct event_base *base = event_base_new()；
	 * 2. 创建/初始化事件
	 *	  调用结束时，事件处于未决状态 
	 *			struct event *ev = event_new(XXXXX);
	 * 3. 将事件插入base  event_add(ev,xxx);
	 * 4. 监听绑定在base上的事件 event_base_dispatch(base); */

	struct event *ev1, *ev2;
	struct timeval five_seconds = {5,0};   // 设置延时
	struct event_base *base = event_base_new();   // 创建base对象

	// 监听fd1是否可读， 并设置超时
	ev1 = event_new(base, fd1, EV_TIMEOUT | EV_READ | EV_PERSIST, cb_func, 
					(char* ) "Reading event");

	// 监听fd2是否可写，并且自动设置成未决，在调用函数时为激活态，结束后为非未决态（此时可选择释放或继续监听事件）
	ev2 = event_new(base, fd2,  EV_WRITE | EV_PERSIST, cb_func, 
					(char* ) "Writing event");

	// 将事件插入base
	event_add(ev1, &five_seconds);
	event_add(ev2, NULL);

	// 循环等待
	// 监听base上绑定的事件，满足后，回调对应函数
	event_base_dispatch(base);
}


int main(int argc, char* argv[])
{
	const char* fifo = "event.fifo";

	int fd1, fd2;

	unlink(fifo);

	mkfifo(fifo, 0644);   // 创建有名管道进行通信

	fd1 = open(fifo, O_RDONLY | O_NONBLOCK, 0644);

	fd2 = open(fifo, O_WRONLY | O_NONBLOCK, 0644);

	main_loop(fd1, fd2);

	return 0;
}
