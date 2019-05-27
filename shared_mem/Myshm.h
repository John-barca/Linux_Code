#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

//static修饰全局变量:改变变量作用域，从整个工程改为当前.c文件
//static修饰函数修改作用域，只在.c文件中生效
//static修饰局部变量，修改生命周期为整个程序
//static修饰类的成员变量,该变量成为类的属性,生命周期为整个程序

static int Createshm()
{
  key_t key = ftok(".",0x1);
  if(key == -1)
  {
    perror("ftok");
    return 1;
  }
  printf("key = %d\n",key);
  int ret = shmget(key, 1024, IPC_CREAT | 0666);
  if(ret < 0)
  {
    perror("shmget");
    return 1;
  }
  printf("ret = %d\n",ret);
  return ret;
}
