#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
    int fd1=open(argv[1],O_RDWR);

    printf("fd1=%d\n",fd1);
    int newfd= fcntl(fd1,F_DUPFD,0);//0被占用，返回当前最小可用文件描述符
    printf("newfd=%d\n",newfd);
    int newfd1=fcntl(fd1,F_DUPFD,7);//7未被占用，返回7，占用返回当前最小可用文件描述符
    printf("newfd1=%d\n",newfd1);

    // int ret=write(newfd1,"777777",6);
    // printf("ret=%d\n",ret);

    close(fd1);

    return 0;
}