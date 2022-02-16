#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(){
	
	for(int i = 0; i<8; i++){
		printf(" parent i = %d\n",i);
	}

	pid_t pid = fork();

	if(pid==0){
		execl("/bin/ls","~","-l",NULL);
	}

	for(int i = 0; i<3; i++){
		printf(" ----------------i = %d\n",i);
	}

	return 0;
}
