#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#define SERV_PORT 8890

 void read_cb(struct bufferevent *bev, void *arg)
 {
	char buf[BUFSIZ];
	bufferevent_read(bev,buf,sizeof(buf));
	printf("client write : %s",buf);

	const char *p="server data to send\n";
	bufferevent_write(bev,p,strlen(p)+1);
	
	sleep(1);
 }

void write_cb(struct bufferevent *bev, void *arg)
{
	printf("server write done\n");
}

void event_cb(struct bufferevent *bev, short what, void *arg)
{
	if(what&BEV_EVENT_EOF)
	{  
        printf("connection closed\n");    
	}
	if(what&BEV_EVENT_ERROR)
	{
		printf("sth error happen\n");
	}

	bufferevent_free(bev);

}

void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
	printf("connect new client\n");
	struct event_base *base=(struct event_base *)arg;
	struct bufferevent * bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);

	bufferevent_enable(bev,EV_READ);

	return;

}

int main(int argc, char *argv[])
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERV_PORT);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	struct event_base *base=event_base_new();

	struct evconnlistener *ev=evconnlistener_new_bind(base,listener_cb,base,LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,36,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

	event_base_dispatch(base);

	evconnlistener_free(ev);
	event_base_free(base);
    
    return 0;
}

