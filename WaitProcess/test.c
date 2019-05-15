#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
  pid_t ret = fork();
  if(ret < 0)
  {
    perror("fork() error\n");
    exit(5);
  }
  else if(ret == 0)
  {
    //child
    sleep(12);
    exit(10);
  }
  else{
    //father
    int status;
    int pid = wait(&status);

    if(pid > 0 && (status & 0x7f) == 0)//正常退出
    {
      printf("child exit code: %d\n",(status >> 8) & 0xff);
    }
    else if(ret > 0)//异常退出
    {
      printf("sig code : %d\n",status & 0x7f);
    }
  }
  return 0;
}
