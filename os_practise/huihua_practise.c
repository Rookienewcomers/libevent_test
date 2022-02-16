#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

void dowork(int no)
{
	// 获取当前时间，并写入文件
	time_t curtime;
	time(&curtime);

	// 格式化时间
	char * ptr = ctime(&curtime);
	int fd = open("/home/haochen/temp---.txt", O_CREAT | O_WRONLY | O_APPEND, 0664);
	write(fd, ptr, strlen(ptr)+1);
}

int main(){
	// 创建会话
	// 父进程无，子进程创建
	pid_t pid = fork();
	if(pid>0)
	{
		// 杀死进程的几种可选方法
		exit(1);
		//kill(getpid(), SIGKILL);
		//raise(SIGKILL);
		//abort();
	}
	else if(pid == 0)
	{
		// 变成会长
		// 成为守护进程
		setsid();
		// 改变进程工作目录
		chdir("/home/haochen");
		// 重置文件掩码
		umask(0);
		// 关闭文件描述符
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		// 执行核心操作
		// 注册信号捕捉
		// signal   sigaction
		struct sigaction act;
		act.sa_flags = 0;
		act.sa_handler = dowork;
		sigemptyset(&act.sa_mask);
		sigaction(SIGALRM, &act, NULL);

		// 例：每隔2S获取系统时间，并写入磁盘文件
		struct itimerval val;
		// 第一次触发的时间
		val.it_value.tv_sec = 2;
		val.it_value.tv_usec = 0;
		// 循环周期
		val.it_interval.tv_sec = 1;
		val.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL, &val, NULL);

		while(1);   // 使进程存活
	}


	return 0;
}
