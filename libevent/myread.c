#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <event2/event.h>
#include <fcntl.h>
#include <sys/stat.h>

void sys_err(const char * str)
{
    perror(str);
    exit(1);
}

void read_cb(evutil_socket_t fd,short what,void *arg)
{
    char buf[BUFSIZ];
    read(fd,buf,sizeof(buf));
    printf("what=read? %s ,read from write : %s\n",what&EV_READ?"YES":"NO",buf);

    sleep(1);

    return;
}

int main(int argc, char *argv[])
{
    int fd;
    unlink("myfifo");
    mkfifo("myfifo",0644);
    fd = open("myfifo",O_RDONLY|O_NONBLOCK);
    if(fd==-1)
        sys_err("open error");
    
    struct event_base * base = event_base_new();

    struct event * event = event_new(base,fd,EV_READ | EV_PERSIST,read_cb,NULL);
    event_add(event,NULL);
    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}

