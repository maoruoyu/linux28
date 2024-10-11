#ifndef _WRAP_H_
#define _WRAP_H_

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void sys_err(const char * str);
int Socket(int domain, int type, int protocol);
int Listen(int sockfd, int backlog);













#endif
