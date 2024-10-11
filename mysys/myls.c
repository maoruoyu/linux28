#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>

int main(int argc,char *argv[])
{
		DIR * pdir;
		struct dirent *prdir;
		pdir=opendir(argv[1]);
		if(pdir==NULL)
		{
			perror("opendir error");
					exit(1);
		}
		while((prdir = readdir(pdir))!=NULL)
		{
			if(strcmp(prdir->d_name,".")==0)
					continue;
			printf("%s\t",prdir->d_name);
		}
		printf("\n");
		
		closedir(pdir);

		return 0;
}

