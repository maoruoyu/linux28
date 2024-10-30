#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define MAXSIZE 2048

void sys_err(const char * str)
{
	perror(str);
	exit(1);
}

int get_line(int fd,char *buf,int size)
{
	int i=0;//索引
	char c='\0';//单次读取字符缓存
	int n;
	while((i<size-1)&&(c!='\n'))//读取不应超出缓冲区大小，读取到换行代表一行结束
	{/*缓冲区结尾添加结束标记'\0'*/
		n=recv(fd,&c,1,0);//单字符读取
		if(n>0)//读到有数据
		{
            if(c=='\r')//读到回车，再次模拟读取下一个字符是否为换行，判断有没有读到行尾
            {
                n=recv(fd,&c,1,MSG_PEEK);
                if((n>0)&&(c=='\n'))//模拟读取回车下个字符为换行，则把换行符读到缓冲区
                {
                    n=recv(fd,&c,1,0);//读换行
                }
                else{
                    c='\n';//强制把回车下个字符标记为换行
                }
            }
            buf[i]=c;//数据放到buf对应位置
            i++;//索引偏移
		}
        else{
            c='\n';//读取结束或出错，强制标记为换行，结束循环
        }     
	}
    buf[i]='\0';//添加结束标记NULL
    if(-1==n)//如果最后一次读取出错，将返回值设为-1表示错误
        i=n;
    return i;
}

// 通过文件名获取文件的类型
const char *get_file_type(const char *name)
{
    char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');   
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}


int init_listen_fd(int port,int epfd)
{
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	if(lfd==-1)
		sys_err("socket error");
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	int opt=1;
	setsockopt(lfd,SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));//设置端口复用

	int ret=bind(lfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(ret==-1)
		sys_err("bind error");
	ret=listen(lfd,128);
	if(ret==-1)
		sys_err("listen error");

	struct epoll_event ev;
	ev.events=EPOLLIN ;
	ev.data.fd=lfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
	if(ret==-1)
		sys_err("epoll_ctl add error");

	return lfd;
}

void disconnect(int fd,int epfd)
{
    int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
    if(ret==-1)
        sys_err("epoll_ctl del error");
    close(fd);
}

void do_accept(int lfd,int epfd)
{
	int ret;
	struct sockaddr_in clit_addr;
	socklen_t clit_addr_len=sizeof(clit_addr);

	int cfd=accept(lfd,(struct sockaddr *)&clit_addr,&clit_addr_len);
	if(cfd==-1)
		sys_err("accept error");
	//打印客户端地址结构
	char client_ip[INET_ADDRSTRLEN]={0};
	printf("NEW CLIENT IP: %s, PORT: %d, CLIENT_FD: %d\n",inet_ntop(AF_INET,&clit_addr.sin_addr,client_ip,sizeof(client_ip)),ntohs(clit_addr.sin_port),cfd);

	int flg=fcntl(cfd,F_GETFL);
	flg|=O_NONBLOCK;
	fcntl(cfd,F_SETFL);//设置cfd非阻塞

	struct epoll_event ev;
	ev.events=EPOLLIN | EPOLLET;//设置边沿触发
	ev.data.fd=cfd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);//将新文件描述符挂到监听树
	if(ret==-1)
		sys_err("epoll_ctl add error");

	return;
}

void send_file(int fd,const char * file)
{
	char buf[4096]={0};
	int len=0,ret=0;
	int rfd=open(file,O_RDONLY);
	if(rfd==-1)
	{
		sys_err("open error");
	}
	while((len=read(rfd,buf,sizeof(buf)))>0)
	{
		ret=send(fd,buf,len,0);
		if(ret==-1)
		{
			if((errno==EAGAIN)||(errno==EWOULDBLOCK))
			{
				perror("send error");
				continue;
			}
			if(errno==EINTR)
			{
				perror("send error");
				continue;
			}
			else
				sys_err("send error");
		}
	}
	if(len==-1)
		sys_err("read file error");
	close(rfd);
}

void send_dir(int fd,const char *file)
{
	//return;
}

void send_respond_head(int fd, int no, const char* desp, const char* type, long len)
{
	char buf[1024]={0};
	sprintf(buf,"HTTP/1.1 %d %s\r\n",no,desp);
	send(fd,buf,strlen(buf),0);//发送消息行

	sprintf(buf,"Content-Type: %s\r\n",type);
	sprintf(buf+strlen(buf),"Content-Length: %ld\r\n",len);
	send(fd,buf,strlen(buf),0);//发送消息头
	//发送空行
	send(fd,"\r\n",2,0);
}

void http_request(int fd,char *request)
{
	char method[12],path[1024],protocol[12];
	//sscanf(request,"%s %s %s",method,path,protocol);//避免路径中有空格分隔错误
	sscanf(request,"%[^ ] %[^ ] %[^ ]",method,path,protocol);
	printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);

	char *file=path+1;//去掉文件路径中的'/',如"/hello.c"->"hello.c"

	if(strcmp(path,"/")==0)
	file="./";

	// 获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if(ret == -1) { 
		sys_err("file stat error");
	}

	if(S_ISDIR(st.st_mode))
	{
		send_respond_head(fd, 200, "OK",get_file_type(".html"), st.st_size);
		send_dir(fd,file);
	}
	else if(S_ISREG(st.st_mode))
	{
		send_respond_head(fd, 200, "OK", "text/plain; charset=utf-8", st.st_size);
		send_file(fd,file);
	}
}

void do_read(int fd,int epfd)
{
    char line[1024];
    int len=get_line(fd,line,sizeof(line));
	if(len==-1)
		sys_err("get_line error");
    else if(len==0)
    {
        printf("client closed...\n");
        disconnect(fd,epfd);
    }
    else{
		char buf[1024];
        printf("请求行消息：%s \n",line);
		printf("请求头: start...\n");
		while(1)
		{
			len=get_line(fd,buf,sizeof(buf));
			if(buf[0]=='\n')
				break;
			else if(len==-1)
				break;
			printf("请求头消息: %s \n",buf);
		}
		printf("请求头: finish...\n");
    }
	if(strncasecmp(line,"GET",3)==0)
	{
		http_request(fd,line);// 处理http请求

		disconnect(fd,epfd);
	}

	return;
}

void epoll_run(int port)
{
	int nready=0,i=0;
	struct epoll_event all_events[MAXSIZE];
	int epfd=epoll_create(MAXSIZE);

	int lfd = init_listen_fd(port,epfd);

	while(1)
	{
		nready=epoll_wait(epfd,all_events,MAXSIZE,0);
		if(nready==-1)
			sys_err("epoll_wait error");
		for(i=0;i<nready;i++)
		{
			struct epoll_event *pev=&all_events[i];
			if(!(pev->events&EPOLLIN))
				continue;
			if(pev->data.fd==lfd)
				do_accept(lfd,epfd);
			else
				do_read(pev->data.fd,epfd);
		}
	}
    return;
}

int main(int argc, char *argv[])
{
	//命令行餐参数获取server提供的目录
	if(argc<3)
	{
		printf("./server port path\n");
	}
	int port = atoi(argv[1]);

	int ret = chdir(argv[2]);//改变进程工作目录
	if(ret<0)
		sys_err("chdir error");

	epoll_run(port);//启动epoll监听

	return 0;
}

