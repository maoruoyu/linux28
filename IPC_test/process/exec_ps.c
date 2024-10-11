#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

int main()
{
	int fd;
	fd=open("ps.out",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd<0)
	{
		perror("open error");
		exit(1);
	}
	dup2(fd,STDOUT_FILENO);
	execlp("ps","ps","aux",NULL);
	perror("execlp error");

	return 0;
}
