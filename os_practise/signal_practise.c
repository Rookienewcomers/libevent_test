#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

int main()
{
	// 自定义信号集
	sigset_t myset;
	
	// 清空集合
	sigemptyset(&myset);

	// 添加要控制的信号
	sigaddset(&myset, SIGINT); // ctrl+c
	sigaddset(&myset, SIGQUIT); // ctrl+反斜杠
	
	// 自定义集合数据设置给内核的阻塞信号集
	sigprocmask(SIG_BLOCK, &myset, NULL);

	while(1)
	{
		sigset_t pendset;
		sigpending(&pendset);  // 读取内核中PCB的未决信号集

		for(int i = 1; i<32; i++)
		{
			if(sigismember(&pendset, i))
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
		}
		printf("\n");
		sleep(1);
	}

	return 0;
}
