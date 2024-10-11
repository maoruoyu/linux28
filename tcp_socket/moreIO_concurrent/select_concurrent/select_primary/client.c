#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

void sys_err(const char * str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int cfd;
	int counter=10;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr;//服务器地址结构体
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&(serv_addr.sin_addr));
	cfd=socket(AF_INET,SOCK_STREAM,0);
	if(cfd==-1)
	{
		sys_err("socket error");
	}
	int ret=connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(ret==-1)
	{
		sys_err("connect error");
	}
	while(--counter)
	{
		write(cfd,"hello\n",6);
		ret=read(cfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,ret);
		sleep(1);
	}
	close(cfd);
    
    return 0;
}

