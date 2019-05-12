#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t ret = fork();
  if(ret > 0)
  {
    // 父进程
     sleep(3);  
     printf("father process pid = %d %d\n",getpid(),getppid());
  }
  else if(ret == 0)
  {
    //子进程
    while(1){
     sleep(3);
     printf("child process pid = %d %d\n",getpid(),getppid());
    }
  }
  else{
    perror("fork()");
  }
  return 0;
}
