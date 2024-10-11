
#include <stdio.h>
int sub(int a,int b)
{
	return a-b;
}


int main()
{
		int a=5,b=10;
		printf("%d-%d=%d\n",a,b,sub(a,b));
		return 0;
}
