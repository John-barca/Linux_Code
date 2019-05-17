#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//input表示带切分命令
//output表示切分结果(字符串数组)
//返回值表示output中包含了几个有效元素
int Split(char input[], char* output[])
{
  //借助strtok实现字符串切分
  char* p = strtok(input, " ");
  int i = 0;
  while(p != NULL)
  {
    //printf("%s\n", p);
    output[i] = p;
    ++i;
    p = strtok(NULL, " ");
  }
  output[i] = NULL;//重要
  return i;
}

//ls -l
//argv[0] = ls

void CreateProcess(char* argv[],int n)
{
  (void)n;
  //创建子进程
  //父进程进行程序等待，子进程进行程序替换
  pid_t ret = fork();
  if(ret > 0)
  {
    //father
    wait(NULL);
  }
  else if(ret == 0)
  {
    //child
    ret = execvp(argv[0], argv);
    //if条件省略，如果exec成功，肯定不会执行下面代码
    perror("exec");
    exit(0);
  }
  else{
    perror("fork");
  }
}

int main()
{
  while(1)
  {
    //打印提示符
    printf("[myshell@localhost ~]$ ");
    fflush(stdout);
    //用户输入一个命令
    char command[1024] = {0};
    gets(command);//scanf遇到空格会出现问题
    //一次读一行数据
    //解析指令,把要执行哪个程序识别出来
    //哪些是命令行参数识别出来(字符串切分)
    //切分结果应该是一个字符串数组
    char* argv[1024] = {0};
    int n = Split(command, argv);
    //创建子进程，进行程序替换
    CreateProcess(argv, n);
  }
  return 0;
}
