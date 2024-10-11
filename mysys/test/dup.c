#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
    int fd=open(argv[1],O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        exit(1);
    }
    int newfd=dup(fd);
    printf("newfd=%d\n",newfd);

    close(fd);
    return 0;
}
