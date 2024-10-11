#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

struct student{
    int id;
    char name[20];
    int age;
};

void sys_err(char * str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    struct student stu={1,"zhangsan",32};
    struct student * p;
    int fd;
    fd = open("mmap_wr",O_RDWR|O_CREAT|O_TRUNC,0644);
    //fd = open("/dev/zero",O_RDWR);
    /*dev/zero 是一个特殊的字符设备，它不占用实际的磁盘空间。
    当使用 /dev/zero 创建映射区时，内核会创建一个匿名的内存区域。
    每次对 /dev/zero 的 mmap 调用都会创建一个新的、独立的匿名内存区域。*/

    if(fd==-1)
    {
        sys_err("open error");
    }
    ftruncate(fd,sizeof(stu));
    p=mmap(NULL,sizeof(stu),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p==MAP_FAILED)
    {
        sys_err("mmap error");
    }
    close(fd);

    while(1)
    {
        memcpy(p,&stu,sizeof(stu));
        stu.id++;
        sleep(2);
    }
    munmap(p,sizeof(stu));
    return 0;
}

