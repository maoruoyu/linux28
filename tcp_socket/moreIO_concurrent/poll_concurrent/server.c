#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <ctype.h>
#include <poll.h>

#include "wrap.h"

#define MAXLINE 80
#define SERV_PORT 8888
#define OPEN_MAX 1024

int main(int argc, char *argv[])
{
    int lfd,cfd,sockfd,i,j,maxi;
    int nready;
    ssize_t n;
    char buf[MAXLINE],str[INET_ADDRSTRLEN];
    socklen_t clit_addr_len;
    struct pollfd client[OPEN_MAX];


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

    client[0].fd=lfd;
    client[0].events=POLLIN;

    for(i=1;i<OPEN_MAX;i++)
    client[i].fd=-1;

    maxi=0;

    for(;;)
    {
        nready=poll(client,maxi+1,-1);
        if (nready < 0)
        perr_exit("poll error");    

        if(client[0].revents&POLLIN)
        {
            clit_addr_len=sizeof(clit_addr);
            cfd=Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
            printf("received at %s ,port: %d\n",inet_ntop(AF_INET,&clit_addr.sin_addr,str,sizeof(str)),ntohs(clit_addr.sin_port));

            for(i=1;i<OPEN_MAX;i++)
            {
                if(client[i].fd<0)
                {
                    client[i].fd=cfd;
                    break;
                }
            }
            if(i==OPEN_MAX)
            perr_exit("too many clients");
            client[i].events=POLLIN;
            if(maxi<i)
            maxi=i;

            if(--nready<=0)
            continue;
        }
        for(i=1;i<maxi+1;i++)    
        {
            if((sockfd=client[i].fd)<0)
            continue;
            if(client[i].revents==POLLIN)
            {
                if((n=Read(sockfd,buf,MAXLINE))<0)
                {
                    if(errno==ECONNRESET)
                    {
                        printf("client[%d] aborted connection\n",i);
                        Close(sockfd);
                        client[i].fd=-1;
                    }
                    else{
                        perr_exit("read error");
                    }
                }
                else if(n==0)
                {
                    printf("client[%d] closed connection\n",i);
                    Close(sockfd);
                    client[i].fd=-1;
                }
                else{
                    for(j=0;j<n;j++)
                    buf[j]=toupper(buf[j]);
                    Writen(sockfd,buf,n);
                }
                if(--nready<=0)
                break;
            }
        }
    }

    return 0;
}

