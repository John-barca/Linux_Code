
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  pid_t ret = fork();
  if(ret < 0)
  {
    perror("Error!");
  }
  else if(ret == 0)
  {
    //子进程
    printf("this child process,pid: %d %d\n",getpid(),getppid());
    int count = 0;
    int i;
    for(i = 0; i < 5; i++)
    {
      count++;
      sleep(1);
    }
    printf("count is %d\n",count);
  }
  else{
    //父进程
    printf("this is father process,pid: %d %d\n",getpid(),getppid());
    wait(NULL);
    printf("this is father process,pid: %d %d\n",getpid(),getppid());
  }

  return 0;
}
