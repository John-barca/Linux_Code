#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>

//void* ThreadEntry(void* arg)
//{
//  (void)arg;
//  while(1){
//    printf("this is ThreadEntry!\n");
//    sleep(1);
//  }
//}
//
//int main()
//{
//  pthread_t tid;
//  pthread_create(&tid, NULL, ThreadEntry, NULL);
//  while(1){
//    printf("this is main_thread,tid is %lu\n",tid);
//    sleep(1);
//  }
//  return 0;
//}
//#define Thread_Num 2
//
//
//
//int main()
//{
//  int* p = (int*)malloc(4*1000000);
//  __int64_t beg = gettimeofday()
//  for(int i = 0; i < 1000000; i++){
//    p[i] = p[i] * p[i];
//  }  
//  return 0;
//}

//void* ThreadEntry (void* arg){
//  (void) arg;
//  while(1){
//    printf("this is ThreadEntry %lu\n",pthread_self());
//    sleep(1);
//  }
//  return NULL;
//}
//
//int main(){
//  pthread_t tid = 0;
//  pthread_create(&tid, NULL, ThreadEntry, NULL);
//  while(1){
//    printf("this main Thread = %lu\n",pthread_self());
//    //pthread_cancel(tid);
//    //pthread_join(tid,NULL);
//    sleep(1);
//  }
//  return 0;
//}
//线程之间共享内存地址

#if 0
int g_count = 0;
void* ThreadEntry(void* arg){
  (void) arg;
  int i;
  for(i = 0; i < 50000; i++){
    g_count++;
  }
  return NULL;
}
int main(){
  pthread_t tid;
  int i;
  for(i = 0; i < 50000; i++){
    g_count++;
  }
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  pthread_join(tid,NULL);
  printf("g_count is %d\n",g_count);
  return 0;
}
#endif
//线程异常终止

#if 0
void* ThreadEntry(void* arg){
  (void) arg;
  while(1){
    sleep(1);
    int *p = NULL;
    *p = 10;
  }
  return NULL;
}

int main(){
  pthread_t tid;
  pthread_create(&tid,NULL,ThreadEntry,NULL);
  while(1){
    printf("this main Thread\n");
    sleep(1);
  }
  return 0;
}
#endif


//3.多线程利用多核资源
#if 0
void* ThreadEntry(void* arg){
  (void) arg;
  while(1);
  return NULL;
}

int main(){
  pthread_t tid1,tid2,tid3,tid4;
  pthread_create(&tid1,NULL,ThreadEntry,NULL);
  pthread_create(&tid2,NULL,ThreadEntry,NULL);
  pthread_create(&tid3,NULL,ThreadEntry,NULL);
  pthread_create(&tid4,NULL,ThreadEntry,NULL);
  while(1);
  return 0;
}
#endif

//通过多线程提高程序执行效率
//假设存在一个很大的数组，把数组中的每一个元素都执行一个乘方运算
//再赋值回数组中
//单线程
#if 0
#define SIZE 1000000

int64_t GetUs(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec*1000000 + tv.tv_usec;
}

void Calc(int *arr, int beg, int end){
  int i = beg;
  for(; i < end; i++){
    arr[i] = arr[i]*arr[i];
  }
}

int main(){
  srand(time(NULL));
  int *arr = (int*)malloc(sizeof(int)*SIZE); //开始时间
  int64_t beg = GetUs();
  Calc(arr,0,SIZE);
  int64_t end = GetUs();//结束时间
  printf("time = > %ld\n",end - beg);//时间做差，得数为所用的时间
  return 0;
}
#endif
//多线程
#define SIZE 1000000
#define THREAD_NUM 2

int64_t GetUs(){
  struct timeval tv;//创建gettimeofday()函数的结构体参数
  gettimeofday(&tv,NULL);
  return tv.tv_sec*1000000 + tv.tv_usec;//得到微秒
}

void Calc(int* arr, int beg, int end){
  int i = beg;
  for(; i < end; i++){
    arr[i] = arr[i]*arr[i];
  }
}

typedef struct Arg{
  int64_t beg;//使得可以将参数传进去
  int64_t end;
  int* arr;
}Arg;

void* ThreadEntry(void* arg){
  Arg* p = (Arg*)arg;
  Calc(p->arr,p->beg,p->end);
  return NULL;
}

int main(){
  srand(time(NULL));
  int* arr = (int*)malloc(sizeof(int) * SIZE);
  pthread_t tid[THREAD_NUM];
  Arg arg[THREAD_NUM];
  int i = 0;
  int min = 0;//中间时间点
  for(; i < THREAD_NUM; i++){
    arg[i].beg = min;
    arg[i].end = min + SIZE/THREAD_NUM;
    arg[i].arr = arr;
    min = min + SIZE/THREAD_NUM;
  }

  int64_t beg = GetUs();//得到开始时间
  int j = 0;//创建j个线程
  for(; j < THREAD_NUM; j++){
    pthread_create(&tid[j],NULL,ThreadEntry,&arg[j]);
  }

  int wid = 0;
  for(;wid < THREAD_NUM; wid++){
    pthread_join(tid[wid],NULL);//进行等待
  }
  int64_t end = GetUs();//得到结束时间
  printf("time is ---> %ld\n",end - beg);//结束时间与开始时间做差得到所用时间
  return 0;
}
