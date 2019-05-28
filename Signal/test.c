#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

//kill系统调用
//int main(int argc, char* argv[])
//{
//  if(argc != 3){
//    //命令行参数超过三个
//    printf("参数有误！./[mykill] [signo] [pid]\n");
//    return 1;
//  }
//
//  int signo = atoi(argv[1] + 1);
//  pid_t pid = atoi(argv[2]);
//  int ret = kill(pid, signo);
//  if(ret < 0)
//  {
//    perror("kill");
//    return 1;
//  }
//  return 0;
//}
//typedef void (*myhandler_t)(int);

//void Myhandler(int signo)
//{
//  printf("signo = %d\n",signo);
//}
//int main()
//{
//  //signal函数替换了信号原有的处理方式
//  //原有的处理方式（就会通过signal的返回值来返回）
//  signal(2, Myhandler);
//  while(1)
//  {
//    sleep(1);
//  }
//  return 0;
//}
//int main()
//{
//  int fd[2] = {0};
//  pipe(fd);
//  close(fd[0]);
//  write(fd[1],"hehe",4);
//  printf("write done!\n");
//  return 0;
//}
//volatile优化时不将变量优化到寄存器里
//volatile int flag = 1;
//
//void Myhandler(int sig){
//  (void)sig;
//  flag = 0;
//}
//
//int main()
//{
//  signal(2,Myhandler);
//  while(flag);
//  return 0;
//}
void Myhandler(int sig)
{
  (void)sig;
  printf("child exit!\n");
  while(1){
    //如果ret>0表示已经回收到了一个进程，返回值就是子进程的pid
    //如果ret == 0表示还有子进程在，但是子进程没结束
    //ret < 0没有子进程
    int ret = waitpid(-1, NULL, WNOHANG);
    if(ret > 0){
      //继续执行循环
    }
    else{
      //退出循环
      break;
    }
  }
}

int main()
{
  signal(SIGCHLD,Myhandler);
  //pid_t ret = fork();
  //if(ret > 0)
  //{
  //  //father
  //  while(1)
  //  {
  //    sleep(1);
  //  }
  //}
  //else if(ret == 0){
  //  //child
  //  exit(0);
  //}else{
  //  perror("fork");
  //}
  for(int i = 0; i < 20; i++)
  {
    pid_t ret = fork();
    if(ret == 0)
    {
      printf("child pid %d\n",getpid());
      exit(0);
    }
    while(1){
      sleep(1);
    }
  }
  return 0;
}
