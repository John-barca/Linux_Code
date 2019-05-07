#include <stdio.h>
#include <unistd.h>
int main()
{
  //__pid_t = pid_t = int
  //等价
  pid_t ret = fork();
  if(ret > 0)
  {
    //父进程
    printf("father:%d\n",getpid());
  }
  else if(ret == 0)
  {
    //子进程
    printf("child:%d\n",getpid());
  }
  else
  {
    perror("fork");
  }
  printf("pid:%d ppid:%d ret:%d\n",getpid(),getppid(),ret); 
  sleep(1);
  return 0;
}
