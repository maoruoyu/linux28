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
#define CLIT_ADDR "clit.socket"

int main(int agrc,char * argv[])
{
    int cfd;
    int n,len;
	char buf[BUFSIZ];
    cfd=Socket(AF_UNIX,SOCK_STREAM,0);

    struct sockaddr_un serv_addr,clit_addr;
    bzero(&clit_addr,sizeof(clit_addr));
    clit_addr.sun_family=AF_UNIX;
    strcpy(clit_addr.sun_path,CLIT_ADDR);
    len=offsetof(struct sockaddr_un,sun_path)+strlen(clit_addr.sun_path);
    unlink(CLIT_ADDR);
    Bind(cfd,(struct sockaddr *)&clit_addr,len);

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family=AF_UNIX;
    strcpy(serv_addr.sun_path,SERV_ADDR);
    len=offsetof(struct sockaddr_un,sun_path)+strlen(serv_addr.sun_path);
    Connect(cfd,(struct sockaddr *)&serv_addr,(socklen_t)len);
    
    printf("Ready connecting ...\n");

    while(fgets(buf,sizeof(buf),stdin)!=NULL)
    {
        Write(cfd,buf,strlen(buf));
        n=Read(cfd,buf,strlen(buf));
        Write(STDOUT_FILENO,buf,n);
    }
    Close(cfd);

    return 0;
}
