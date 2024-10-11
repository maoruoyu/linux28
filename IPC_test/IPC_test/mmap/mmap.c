#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

void sys_err(char * str)
{
	perror(str);
	exit(-1);
}

int main(int argc, char *argv[])
{
	char *p=NULL;
	int fd;
	fd=open("testmmap",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd==-1)
	{
		sys_err("open error");
	}
	//lseek(fd,10,SEEK_END);
	//write(fd,"\0",1);
	ftruncate(fd,20);//扩展文件大小到20、需要文件写权限
	int len=lseek(fd,0,SEEK_END);//获取文件大小
	
   	p=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p==MAP_FAILED)
	{
		sys_err("mmap error");
	}

	strcpy(p,"mmap write2");
	printf("%s\n",p);

	int ret = munmap(p,len);
	if(ret==-1)
	{
		sys_err("munmap error");
	}

	close(fd);

    return 0;
}

