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

#define SERV_PORT 8888

struct s_info {                     //定义一个结构体, 将地址结构跟cfd捆绑
    struct sockaddr_in cliaddr;
    int connfd;
};

void *thread_func(void * arg)
{
    char buf[BUFSIZ];
    char str[INET_ADDRSTRLEN];//#define INET_ADDRSTRLEN 16  可用"[+d"查看
    int n,i ;
    struct s_info *ts=(struct s_info *)arg;
    while(1)
    {
        n=Read(ts->connfd,buf,sizeof(buf));
        if(n==0)
        {
            printf("the client %d closed\n",ts->connfd);
            break;
        }
        printf("receive from %s at port %d\n",inet_ntop(AF_INET,&(ts->cliaddr.sin_addr),str,(socklen_t)sizeof(str)),ntohs(ts->cliaddr.sin_port));
        for(i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        Write(STDOUT_FILENO,buf,n);
        Write(ts->connfd,buf,n);

    }
    Close(ts->connfd);
    return (void *)0; //pthread_exit((void *)0);
}

int main(int agrc,char * argv[])
{
    int lfd,cfd;
    int i=0;
    pthread_t tid;
    struct sockaddr_in serv_addr,clit_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    struct s_info ts[256];

    lfd=Socket(AF_INET,SOCK_STREAM,0);
    Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    Listen(lfd,128);
    printf("Accepting client connect ...\n");

    while(1)
    {
        socklen_t clit_addr_len=sizeof(clit_addr);
        cfd=Accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
        ts[i].cliaddr=clit_addr;
        ts[i].connfd=cfd;
        pthread_create(&tid,NULL,thread_func,(void *)&ts[i]);

        pthread_detach(tid);
        i++;
    }

    return 0;
}