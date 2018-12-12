#include <stdio.h>
#include <stdlib.h>

int my_cmp(int a, int b)
{
	int m = 0;
	m = a ^ b;
	int count = 0;
	while (m)
	{
		if ((m & 1) == 1)
		{
			count++;
		}
		m = m >> 1;
	}
	return count;
}
int main()
{
	int a = 0;
	int b = 0;
	printf("请输入两个数:\n");
	scanf("%d%d", &a, &b);
	int ret = my_cmp(a, b);
	printf("有%d个位不同\n",ret);
	return 0;
}
