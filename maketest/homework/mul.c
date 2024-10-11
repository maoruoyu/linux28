
#include <stdio.h>
int mul(int a,int b)
{
	return a*b;
}


int main()
{
		int a=5,b=10;
		printf("%d*%d=%d\n",a,b,mul(a,b));
		return 0;
}
