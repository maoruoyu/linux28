#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/epoll.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8888
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
    int lfd,cfd,sockfd,epfd,i,j;
    int nready;
    ssize_t n;
    char buf[MAXLINE],str[INET_ADDRSTRLEN];
    socklen_t clit_addr_len;

    lfd=Socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));//设置端口复用

    struct sockaddr_in serv_addr,clit_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    Listen(lfd,128);
    
    epfd=epoll_create(OPEN_MAX);
    if(epfd==-1)
    perr_exit("epoll_create error");

    struct epoll_event epev,Wepev[OPEN_MAX];
    epev.events=EPOLLIN;
    epev.data.fd=lfd;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev)==-1)
    perr_exit("epoll_ctl error");

    for(;;)
    {
        nready=epoll_wait(epfd,Wepev,OPEN_MAX,-1);
        if(nready==-1)
        perr_exit("epoll_wait error");
        for(i=0;i<nready;i++)
        {
            if(!Wepev[i].events&EPOLLIN)
            continue;
            if((sockfd=Wepev[i].data.fd)==lfd)
            {
                clit_addr_len=sizeof(clit_addr),
                cfd=Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
                printf("received at %s ,port: %d\n",inet_ntop(AF_INET,&clit_addr.sin_addr,str,sizeof(str)),ntohs(clit_addr.sin_port));

                epev.data.fd=cfd;
                if(epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev)==-1)
                perr_exit("epoll_ctl error");
            }else{
                if((n=Read(sockfd,buf,sizeof(buf)))<0)
                {
                    if(errno==ECONNRESET)
                    {
                        Close(sockfd);
                    }
                    else{
                        perr_exit("read error");
                    }
                }
                else if(n==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                    Close(sockfd);
                }
                else
                {
                    for(j=0;j<n;j++)
                    buf[j]=toupper(buf[j]);
                    Writen(sockfd,buf,n);
                }
            }

        }
    }
    Close(epfd);
    Close(lfd);

    return 0;
}

