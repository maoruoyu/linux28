#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/select.h>

#include "wrap.h"

#define SERV_PORT 6666

int main(int argc,char *argv[])
{
	int lfd,cfd,maxfd;
	pid_t pid;
	int i,n,j;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr,clit_addr;
	socklen_t clit_addr_len;

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

	int nready;
	fd_set rset,allset;
	maxfd=lfd;
	FD_ZERO(&allset);
	FD_SET(lfd,&allset);//将监听连接读事件添加到监听事件集合
	while(1)
	{
		rset=allset;//使用allset保存原始rset集合，防止select函数传出值将原始集合修改
		nready=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nready<0)
			perr_exit("select error");
		if(FD_ISSET(lfd,&rset))
		{
			clit_addr_len=sizeof(clit_addr);
			cfd=Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
			FD_SET(cfd,&allset);//将数据读事件添加到监听事件集合
			if(maxfd<cfd)
				maxfd=cfd;
			if(0==--nready)//nready=1,说明select返回为1，并且为lfd，后续无需执行
				continue;
		}
		for(i=lfd+1;i<=maxfd;i++)
		{
			if(FD_ISSET(i,&rset))
			{
				if((n=Read(i,buf,sizeof(buf)))==0)
				{
					FD_CLR(i,&allset);
					Close(i);
				}
				else if(n>0){
					for(j=0;j<n;j++)
						buf[j]=toupper(buf[j]);
					Write(i,buf,n);
				}

			}

		}
	}

	close(lfd);

	return 0;
}
