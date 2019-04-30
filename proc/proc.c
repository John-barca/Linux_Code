#include <stdio.h>
#include <unistd.h>

int main()
{
	int i = 0;
	char bar[102]={'\0'};
	const char *label="|/-\\";
	for(;i <= 100; i++)
	{
	        bar[i]='#';
		printf("[%-100s][%d%%][%c]\r",bar,i,label[i%4]);
		fflush(stdout);
		usleep(100000);
	}
	printf("\n");
	return 0;
}
