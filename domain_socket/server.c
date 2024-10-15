#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_ADDR "serv.socket"

int main(int agrc,char * argv[])
{
    int lfd,cfd;
    int i,n,len;
	char buf[BUFSIZ];
    lfd=Socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un serv_addr,clit_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family=AF_UNIX;
    strcpy(serv_addr.sun_path,SERV_ADDR);
    len=offsetof(struct sockaddr_un,sun_path)+strlen(serv_addr.sun_path);
    unlink(SERV_ADDR);
    
    Bind(lfd,(struct sockaddr *)&serv_addr,len);//本地套接字bind函数调用成功会创建一个套接字，所以需要先unlink这个套接字文件再bind
    Listen(lfd,20);
    printf("Accepting client connect ...\n");

    while(1)
    {
        //socklen_t clit_addr_len=sizeof(clit_addr);
        len=sizeof(clit_addr);
		cfd=Accept(lfd,(struct sockaddr *)&clit_addr,(socklen_t *)&len);
        len-=offsetof(struct sockaddr_un,sun_path);
        clit_addr.sun_path[len]='\0';
        printf("client bind domain name:%s\n",clit_addr.sun_path);

		while((n=read(cfd,buf,sizeof(buf)))>0){
            for(i=0;i<n;i++)
            {
                buf[i]=toupper(buf[i]);
            }
            Write(STDOUT_FILENO,buf,n);
            Write(cfd,buf,n);
        }
        Close(cfd);
    }

    Close(lfd);
    return 0;
}
