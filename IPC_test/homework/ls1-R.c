#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>


void fetchdir(char *path);
void isFile(char *path)
{
    struct stat sbuf;
    int ret=stat(path,&sbuf);
    if(ret==-1)
    {
        perror("stat error");
        return;
    }
    if(S_ISDIR(sbuf.st_mode))
    {
        fetchdir(path);
    }
    printf("%8ld %s\n",sbuf.st_size,path);
    
}

void fetchdir(char *path)
{
    char name[256];
    DIR *dir;
    struct dirent *sdp;
    dir=opendir(path);
    if(dir==NULL)
    {
        perror("opendir error");
        return;
    }
    while((sdp=readdir(dir))!=NULL)
    {
        if(strcmp(sdp->d_name,".")==0||strcmp(sdp->d_name,"..")==0)
            continue;
        if(strlen(sdp->d_name)+strlen(path)+2>256)
        {
            fprintf(stderr,"path too long\n");
			return;
        }
        else
        {
            sprintf(name,"%s/%s",path,sdp->d_name);
        isFile(name);
        }
    }
    closedir(dir);
}


int main(int argc,char *argv[])
{
    if(argc==1)
    {
        isFile(".");
    }
    else
    {
        while(--argc)
        {
            isFile(*++argv);
        }
    }
    return 0;
}
