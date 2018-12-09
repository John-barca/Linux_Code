#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
int my_strlen(const char *str)
{
	int count = 0;
	assert(str);
	while (*str != '\0')
	{
		count++;
		str++;
	}
	return count;
}
void reverse(char a[],int left,int right)
{
	while (left < right)
	{
		char tmp = a[left];
		a[left] = a[right];
		a[right] = tmp;
		left++;
		right--;
	}
}
char* reverse_string(char arr[])
{
	int i = 0;
	int left = 0;
	int right = my_strlen(arr) - 1;
	reverse(arr, left, right);
	while ((arr[i]) !='\0')
	{
		left = i;
		while ((arr[i] != '\0') && (arr[i] != ' '))
		{
			i++;
		}
		right = i - 1;
		reverse(arr, left, right);
		if (arr[i] != '\0')
		{
			i++;
		}
	}
	return arr;
}
int main()
{
	char arr[] = "student a am i";
	reverse_string(arr);
	printf("%s\n", arr);
	return 0;
}
