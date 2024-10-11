#include <stdio.h>
int div1(int a,int b)
{
	return a/b;
}


int main()
{
		int a=5,b=10;
		printf("%d/%d=%d\n",a,b,div1(a,b));
		return 0;
}
