#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc,char *argv[])
{
   int ret = truncate("dict.cp",100);
   printf("ret=%d\n",ret);
	return 0;
}
