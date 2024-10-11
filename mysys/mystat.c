#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
		struct stat sbuf;
		int ret=stat(argv[1],&sbuf);
		if(ret==-1)
		{
				perror("stat err");
			exit(1);
		}		
		printf("file size：%ld\n",sbuf.st_size);
		if(S_ISREG(sbuf.st_mode))
		{
				printf("file is regular\n");
		}
		else if(S_ISDIR(sbuf.st_mode))
		{
				printf("file is belong to dir\n");
		}
		else if(S_ISFIFO(sbuf.st_mode))
		{
				printf("file is soft ln  \n");
		}
		return 0;
}
