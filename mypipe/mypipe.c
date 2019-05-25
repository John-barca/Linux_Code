#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//int main()
//{
//  int fd[2];
//  if(pipe(fd) < 0)
//  {
//    perror("pipe");
//    exit(3);
//  }
//  
//  pid_t ret = fork();
//  if(ret == 0)
//  {
//    //child
//    close(fd[0]);
//    char* buf = "this is child!\n";
//    int i = 0;
//    while(1)
//    {
//      write(fd[1],buf,sizeof(buf) - 1);
//    }
//      printf("child process write done!%d\n", i++);
//    close(fd[1]);
//    exit(0);
//  }
//  else if(ret > 0)
//  {
//    //father
//    close(fd[1]);
//    char buf[1024] = {0};
//    sleep(3);
//    wait(NULL);
//    printf("this is father process %d\n",getpid());
//    while(1)
//    {
//      ssize_t s = read(fd[0],buf,sizeof(buf)-1);
//      if(s > 0)
//      {
//        printf("father read done: buf %s\n",buf);
//        exit(1);
//      }
//      else if(s == 0)
//      {
//        printf("quit");
//        break;
//      }
//    }
//  }
//  return 0;
//}
int main()
{
  int fd[2];
  if(pipe(fd) == -1)
  {
    perror("pipe");
  }

  pid_t pid = fork();
  if(pid == -1)
  {
    perror("fork");
  }
  if(pid == 0)
  {
    //child
    close(fd[0]);
    write(fd[1], "hello", 5);
    close(fd[1]);
    printf("child write done\n");
  }
  else if(pid > 0)
  {
    //father
    close(fd[1]);
    char buf[10] = {0};
    read(fd[0], buf, 10);
    printf("read done buf = -> %s\n",buf);
  }
  return 0;
}
