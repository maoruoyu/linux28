#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/select.h>

#include "wrap.h"

#define SERV_PORT 8192
#define MAX_CLIENTS FD_SETSIZE

int findcfd(int arr[], int size, int target) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == target) {
			return i;  // 找到目标，返回索引
		}
	}
	return -1;  // 未找到目标，返回 -1
}


int main(int argc, char *argv[])
{
	int i, ret, cfdNum = 0, maxfd, listenfd, connfd, sockfd;
	int nready, client[MAX_CLIENTS];
	ssize_t n;
	fd_set rset, allset;
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];
	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	if (listen(listenfd, 20) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	maxfd = listenfd;
	for (i = 0; i < MAX_CLIENTS; i++)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	while (1) {
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready < 0) {
			perror("select error");
			continue;
		}

		if (FD_ISSET(listenfd, &rset)) {
			cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
			if (connfd < 0) {
				perror("accept error");
				continue;
			}
			printf("New connection from %s at PORT %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));



			// if (i > maxi) {
			//     maxi = i;
			// }
			FD_SET(connfd, &allset);
			if(cfdNum<MAX_CLIENTS)
				client[cfdNum++]=connfd; // 增加连接计数
			else {
				fputs("too many clients\n", stderr);
				exit(1);
			}

			if (connfd > maxfd)
				maxfd = connfd;
			if (--nready == 0)
				continue;
		} 

		for (i = 0; i < cfdNum; i++) {
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ((n = read(sockfd, buf, BUFSIZ)) == 0) {
					// 客户端关闭连接
					close(sockfd);
					FD_CLR(sockfd, &allset);
					ret=findcfd(client,cfdNum,sockfd);
					if(ret!=-1)
					{
						for(int j=ret;j<cfdNum;j++)
						{
							client[j]=client[j+1];
						}
						cfdNum--;  // 减少连接计数
					}
					// if (sockfd == maxfd) {
					// 	// 找到新的最大文件描述符
					// 	maxfd = listenfd;  // 从监听套接字开始
					// 	for (int j = 0; j < cfdNum; j++) {
					// 		if (client[j] > maxfd) {
					// 			maxfd = client[j];
					// 		}
					// 	}
					// }可以不更新

					// // 更新 maxi 如果需要
					// if (i == maxi) {
					//     while (maxi >= 0 && client[maxi] == -1)
					//         maxi--;
					// }
				} else if (n > 0) {
					printf("Received: %.*s\n", (int)n, buf);  // 打印接收到的数据
					for (int j = 0; j < n; j++)
						buf[j] = toupper((unsigned char)buf[j]);  // 确保正确转换
					printf("Converted: %.*s\n", (int)n, buf);  // 打印转换后的数据
					if (write(sockfd, buf, n) != n) {
						perror("write error");
					}
				}
				if (--nready == 0)
					break; 
			}
		}
	}

	close(listenfd);
	return 0;
}
