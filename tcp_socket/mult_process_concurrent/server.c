#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>

#include "wrap.h"

#define SERV_PORT 9999

void catch_child(int signum)
{
	while(waitpid(0,NULL,WNOHANG)>0);
	return;
}

int main(int argc,char *argv[])
{
	int lfd,cfd;
	pid_t pid;
	int ret,i;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr,clit_addr;
	socklen_t clit_addr_len;
	
	struct sigaction sig_Act;
	sig_Act.sa_handler=&catch_child;
	sigemptyset(&sig_Act.sa_mask);
	sig_Act.sa_flags=0;
	
	//memset(&serv_addr,0,sizeof(serv_addr));
	bzero(&serv_addr,sizeof(serv_addr));//使用bzero函数（memset功能）将serv_addr清零
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERV_PORT);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	lfd=Socket(AF_INET,SOCK_STREAM,0);
	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));//设置端口复用
	Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	Listen(lfd,128);
	
	while(1)
	{
		clit_addr_len=sizeof(clit_addr);
		cfd=Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
		pid=fork();
		if(pid<0)
		{
			perr_exit("fork error");
		}
		else if(pid==0)
		{
			Close(lfd);
			break;
		}
		else{
			ret=sigaction(SIGCHLD,&sig_Act,NULL);
			if(ret==-1)
			{
				perr_exit("sigaction error");
			}
			Close(cfd);
			continue;
		}

	}
	if(pid==0)
	{
		for(;;){
		ret=Read(cfd,buf,sizeof(buf));
		if(ret==0)//ret==0,表示客户端关闭连接，此时可以关闭套接字描述符退出
		{
			Close(cfd);
			exit(1);
		}
		for(i=0;i<ret;i++)
		{
			buf[i]=toupper(buf[i]);
		}
		Write(cfd,buf,ret);
		Write(STDOUT_FILENO,buf,ret);
		}
	}
	return 0;
}
