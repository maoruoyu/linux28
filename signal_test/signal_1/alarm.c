#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    int i;
	alarm(1);
	for(i=0;;i++)
		printf("%d\n",i);
    return 0;
}

