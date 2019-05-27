#include "Myshm.h"
#include <string.h>

int main()
{
  //往共享内存里写数据
  //1.创建/打开共享内存对象
  int shmid = Createshm(); 
  //2.附加到共享内存对象上
  char* p = (char*)shmat(shmid, NULL, 0);
  //3.直接使用
  strcpy(p, "This is Myshared_memory\n");
  return 0;
}
