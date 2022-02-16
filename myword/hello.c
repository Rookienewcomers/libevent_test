#include <stdio.h>
#include "head.h"
int main(){
	int a = 10;
	int b = 5;
	//int c = a + b;
	int c = add(a,b);
	//int d = a - b;
	int d = sub(a,b);
	printf("a+b=%d  a-b=%d \n",c,d);
	return 0;
}

