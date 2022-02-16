#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <arpa/inet.h>

void read_cb(struct bufferevent* bev, void* ctx)
{
	// 如果客户端有数据写过来，则触发该函数
	
	// input为当前bufferevent的输入缓冲区地址，从input中获取客户端数据
	struct evbuffer* input = bufferevent_get_input(bev);

	// output为当前bufferevent的输出缓冲区地址，如果想向客户端写数据，则将数据写到output即可 
	struct evbuffer* output = bufferevent_get_output(bev);

	// 将读缓冲 input 数据直接写入写缓冲区域 output
	evbuffer_add_buffer(output, input);
}

void accept_conn_cb(struct evconnlistener* listener,
					evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	// fd----服务器已经accept成功后的cfd，可直接与客户端进行通信的套接字
	
	struct event_base* base = evconnlistener_get_base(listener);

	// 创建bufferevent 绑定fd和base
	struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	// 注册buffervent事件的回调函数（读/写/错误，可为空(NULL)）
	bufferevent_setcb(bev, read_cb, NULL, NULL, NULL);

	// 启动监听的bufferevent的读和写事件
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

int main(int arg, char* argv[])
{
	struct event_base* base = event_base_new();
	struct sockaddr_in sin;
	struct evconnlistener* listener;

	int port = 9995;  // 定义端口号

	// 用0清空初始化
	memset(&sin, 0, sizeof(sin));
	// 地址族，一般为AF_XXX，PF_XXX形式，大多采用AF_INET地址，代表TCP/IP协议族
	sin.sin_family = AF_INET;
	// IP地址，初始化为0.0.0.0
	sin.sin_addr.s_addr = htonl(0);
	// 端口号
	sin.sin_port = htons(port);

	listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 
									   -1, (struct sockaddr*) &sin, sizeof(sin));

	event_base_dispatch(base);

	event_base_free(base);

	return 0;
}

