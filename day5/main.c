#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

int main()
{
	printf("Hello, world!\n");
	fflush(NULL);
	_exit(0);
}

