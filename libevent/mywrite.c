#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <fcntl.h>

void sys_err(const char * str)
{
    perror(str);
    exit(1);
}

void write_cb(evutil_socket_t fd,short what,void *arg)
{
    char buf[]="hello libevent";

    write(fd,buf,strlen(buf)+1);

    sleep(1);

    return;
}

int main(int argc, char *argv[])
{
    int fd;
    fd = open("myfifo",O_WRONLY|O_NONBLOCK);
    if(fd==-1)
        sys_err("open error");
    
    struct event_base * base = event_base_new();

    struct event * event = event_new(base,fd,EV_WRITE | EV_PERSIST,write_cb,NULL);
    event_add(event,NULL);
    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}
