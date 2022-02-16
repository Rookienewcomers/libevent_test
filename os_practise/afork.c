#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
int count = 10;

int main(int argc, const char* argv[]){
	int i = 0;
	int number = 3;
	pid_t pid;

	for( i=0; i<number; i++)
	{
		pid = fork();
		if(pid == 0){
			break;}

	}

	if(i==0){
		printf("first process, pid = %d\n",getpid());
		printf("conut = %d\n", count++);
	}
	if(i==1){
		printf("second process, pid = %d\n",getpid());
		printf("conut = %d\n", count++);
	}
	if(i==2){
		printf("third process, pid = %d\n",getpid());
		printf("conut = %d\n", count++);
	}
	if(i==number){
		printf("parent process, pid = %d\n",getpid());
		printf("conut = %d\n", count++);
	}

		return 0;
}
