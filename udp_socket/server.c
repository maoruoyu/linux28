#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_PORT 8888

/*
没有换行符（\n）可能导致输出不立即显示的原因涉及到 C 语言的输出缓冲机制。这里有几个关键点需要理解：
缓冲机制：
C 语言的标准输出（stdout）通常是行缓冲的。这意味着输出会被存储在一个缓冲区中，直到遇到换行符或缓冲区满了。
缓冲区刷新：
当遇到换行符（\n）时，缓冲区会被刷新，内容会立即输出到屏幕。
如果没有换行符，内容可能会留在缓冲区中，直到缓冲区满或程序结束。
为什么这会影响 UDP 服务器：
在服务器程序中，主循环持续运行，可能长时间不会结束。
如果 printf 语句没有换行符，输出可能会一直留在缓冲区中，不会显示在屏幕上。
解决方法：
添加换行符：printf("... \n");
使用 fflush(stdout); 在 printf 后强制刷新缓冲区。
禁用缓冲：setbuf(stdout, NULL); 在程序开始时调用。
为什么这很重要：
在网络编程中，及时看到输出对于调试和监控程序行为非常重要。
没有即时输出可能会误导开发者，让人以为程序没有正确运行。
6. 最佳实践：
在服务器程序中，总是在日志或调试输出语句末尾添加换行符。
考虑使用专门的日志库，它们通常会自动处理缓冲和刷新。
总结：
添加换行符不仅使输出更易读，还确保了输出会立即显示，这在服务器程序中特别重要。它帮助开发者实时监控程序的行为，是一个简单但重要的编程习惯。
*/

int main(int agrc,char * argv[])
{
    int lfd;
    int i=0,n=0;
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];
    struct sockaddr_in serv_addr,clit_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    lfd=Socket(AF_INET,SOCK_DGRAM,0);
    Bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    //Listen(lfd,128);
    printf("Accepting client connect ...\n");

    while(1)
    {
        socklen_t clit_addr_len=sizeof(clit_addr);
		n=recvfrom(lfd,buf,BUFSIZ,0,(struct sockaddr *)&clit_addr,&clit_addr_len);
		if(n==-1)
			perr_exit("recvfrom error");

		//printf("receive from %s at port %d",inet_ntop(AF_INET,&clit_addr.sin_addr,str,sizeof(str)),ntohs(clit_addr.sin_port));
        printf("receive from %s  at port:%d\n",inet_ntop(AF_INET,&clit_addr.sin_addr,str,sizeof(str)),ntohs(clit_addr.sin_port));
		for(i=0;i<n;i++)
			buf[i]=toupper(buf[i]);
		Write(STDOUT_FILENO,buf,n);
		n=sendto(lfd,buf,n,0,(struct sockaddr *)&clit_addr,sizeof(clit_addr));
		if(n==-1)
			perror("sendto error");
    }

	Close(lfd);
    return 0;
}
