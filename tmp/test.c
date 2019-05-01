#include <stdio.h>
#include <unistd.h>

#define END "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define DGREEN "\033[36m"
#define WHITE "\033[37m"

int main()
{
  const char* lib = "/-\\|";
  char buf[102]={0};
  int i = 0;
  for(;i < 100; i++)
  {
    buf[i]='#';
    // \r把光标定义在同一行的开头位置
    int c = i%8;
    switch(c)
    {
      case 0:
        printf(BLACK);
        break;
      case 1:
        printf(RED);
        break;
      case 2:
        printf(GREEN);
        break;
      case 3:
        printf(YELLOW);
        break;
      case 4:
        printf(BLUE);
        break;
      case 5:
        printf(DGREEN);
        break;
      case 6:
        printf(PURPLE);
        break;
      case 7:
        printf(WHITE);
        break;
    }
    printf("[%d%%][%c][%s]\r",i,lib[i%4],buf);
    fflush(stdout);
    usleep(100 * 1000);
  }
  return 0;
}
