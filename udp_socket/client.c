#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_IP   "127.0.0.1"
#define SERV_PORT 8888

int main(int argc, char *argv[])
{
	int cfd;
	int n=0;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr;
	
	cfd=Socket(AF_INET,SOCK_DGRAM,0);
	bzero(&serv_addr, sizeof(serv_addr));  
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr);
	
	//Bind(cfd,(struct sockaddr *)serv_addr,sizeof(serv_addr));  /*无效*/
	while(fgets(buf,BUFSIZ,stdin)!=NULL)
	{
		n=sendto(cfd,buf,strlen(buf),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
		if(n==-1)
			perr_exit("sendto error");
		n=recvfrom(cfd,buf,BUFSIZ,0,NULL,0);
		if(n==-1)
			perr_exit("recvfrom error");
		Write(STDOUT_FILENO,buf,n);
	}

    Close(cfd);
    return 0;
}

