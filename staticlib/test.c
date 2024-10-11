#include <stdio.h>
#include "mymath.h"
int main()
{
	int a=4,b=6;
	printf("%d+%d=%d\n",a,b,add(a,b));
	printf("%d-%d=%d\n",a,b,sub(a,b));
	printf("%d/%d=%d\n",a,b,div1(a,b));
	printf("test ok...\n");
	return 0;
}

