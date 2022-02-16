#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("temp", O_CREAT | O_RDWR, 0664);
	if(fd == -1)
	{
		perror("open file:temp erro");
		exit(1);
	}

	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork erro");
		exit(1);
	}
	if(pid >0)
	{
		char *p= "HELLO WORD!!!";
		write(fd, p, strlen(p)+1);
		close(fd);
	}else if(pid == 0)
	{
		sleep(1);
		char buf[1024];
		lseek(fd, 0, SEEK_SET);
		int len = read(fd, buf, sizeof(buf));
		printf("%s\n", buf);
		close(fd);
	}
	return 0;
}





