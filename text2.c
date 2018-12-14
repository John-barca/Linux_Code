#include <stdio.h>

int main()
{
	int i=0;
	int count = 0;
	for(i= 0; i< 100; i++ )
	{
		if(i % 10 == 9)
		{
			printf("%d\t",i);
			count++;
		}
		if(i / 10== 9)
		{
			printf("%d\t",i);
			count++;
		}
	}
	printf("Number 9 counts:%d\n",count);
	return 0;
}
