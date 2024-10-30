#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <arpa/inet.h>

#define SERV_PORT 8890

void read_cb(struct bufferevent *bev, void *ctx)
{
    char buf[1024]={0};
    bufferevent_read(bev,buf,sizeof(buf));
    printf("receive server data :%s\n",buf);
    //bufferevent_write(bev,buf,strlen(buf)+1);
    sleep(1);
}

void write_cb(struct bufferevent *bev, void *ctx)
{
    printf("finish write to server\n");
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    if(what&BEV_EVENT_EOF)
	{  
        printf("connection closed\n");    
	}
	if(what&BEV_EVENT_ERROR)
	{
		printf("sth error happen\n");
	}
    if(what&BEV_EVENT_CONNECTED)
    {
        printf("success to connect server\n");
        return;
    }
    bufferevent_free(bev);
    return;
}

void read_terminal(evutil_socket_t fd, short event, void *arg)
{
    char buf[1024]={0};
    int len=read(fd,buf,sizeof(buf));
    struct bufferevent * bev=(struct bufferevent *)arg;
    bufferevent_write(bev,buf,len+1);
}

int main(int argc, char *argv[])
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    struct event_base * base=event_base_new();

    struct bufferevent *bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);

    bufferevent_socket_connect(bev,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);
    bufferevent_enable(bev,EV_READ);

    struct event *ev=event_new(base,STDIN_FILENO,EV_READ | EV_PERSIST,read_terminal,bev);
    event_add(ev,NULL);

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}

