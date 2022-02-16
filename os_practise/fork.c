#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, const char* argv[]){
	pid_t pid;
	for(int i=0; i<4; i++)
	{
		printf("-------i = %d\n", i);
	}

	pid = fork();
	if(pid>0){
		printf("parent process, pid = %d\n",getpid());
	}else if(pid==0){
		printf("child process, pid = %d, ppid = %d\n",getpid(),getppid());
	}

	for(int i=0; i<4; i++)
	{
		printf("i = %d\n", i);
	}

	return 0;
}
