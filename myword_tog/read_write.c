#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char* argv[]){
	// 打开文件
	int fd = open("english.txt",O_RDWR);
	printf(" fd = %d\n",fd);

	int fd1 = open("temp", O_WRONLY | O_CREAT, 0664);
	printf(" fd1 = %d\n",fd1);

	char buf[5];
	int len = read(fd, buf, sizeof(buf));

	while( len>0 ){
		int ret = write(fd1, buf, len);
		printf("ret = %d\n", ret);
		len = read(fd, buf, sizeof(buf));
	}
	close(fd);
	close(fd1);
	return 0;
}
