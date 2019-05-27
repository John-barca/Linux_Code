#include "Myshm.h"

int main()
{
  //从共享内存里读数据
  //1.创建/打开共享内存对象
  int shmid = Createshm();  
  //2.附加到共享内存对象上(shmat-->attach,用法和malloc十分相似)
  char* p = (char*)shmat(shmid, NULL, 0);
  //3.使用共享内存
  printf("[reader]:%s\n",p);
  return 0;
}
