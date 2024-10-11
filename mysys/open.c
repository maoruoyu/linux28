#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;
	fd=open("./dict.cp",O_RDONLY|O_CREAT|O_TRUNC,0644);
	printf("fd=%d\n",fd);
	close(fd);

	return 0;
	
}
