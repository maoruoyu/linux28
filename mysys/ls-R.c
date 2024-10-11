#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_LEN 256

void isfile(char *name);

void fetchditr(char *dir)
{
	char name[PATH_LEN];
	DIR * pd;
	struct dirent * sdp;
	pd=opendir(dir);
	if(pd==NULL)
	{
		perror("opendir error");
		return;
	}
	while((sdp=readdir(pd))!=NULL)
	{
		if(strcmp(sdp->d_name,".")==0||strcmp(sdp->d_name,"..")==0)
		{
			continue;
		}
		if(strlen(dir)+strlen(sdp->d_name)+2>PATH_LEN)
		{
			fprintf(stderr,"path too long\n");
			return;
		}
		else{
			sprintf(name,"%s/%s",dir,sdp->d_name);
		isfile(name);
		}
	}
	closedir(pd);
	
}

void isfile(char *name)
{
	int ret;
	struct stat sbuf;
	ret=stat(name,&sbuf);

	if (ret==-1)
	{
		perror("stat error");
		return;
	}
	if(S_ISDIR(sbuf.st_mode))
	{
		fetchditr(name);
	}
	printf("%8ld %s\n", sbuf.st_size, name);   //不是目录,则是普通文件,直接 (打印文件名)

	return;
}

int main(int argc,char * argv[])
{	
	if(argc==1)
	{
		isfile(".");
	}
	else
		while(--argc)
		{
			isfile(*++argv);
		}
	
		

	return 0;
}
