#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <pthread.h>

//实现一个生产者消费者模型
//首先得有一个消费场所
std::vector<int> data;
pthread_mutex_t lock;
pthread_cond_t cond;

//然后得有两个角色，（生产者和消费者）两个线程

void* Product(void* arg){
  (void) arg;
  //负责把数据往交易场所中去放
  
  int count = 0;

  while(1){
    pthread_mutex_lock(&lock);
    data.push_back(++count);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
    usleep(789789);
  }
  return NULL;
}

void* Consume(void* arg){
  (void) arg;
  //负责把数据从交易场所中取出来
  
  while(1){
    //每次取最后一个元素
    pthread_mutex_lock(&lock);

    if(data.empty()){
      //1.先释放锁
      //2.等待条件就绪（有其他线程调用pthread_cond_signal）
      //3.如果条件就绪了，重新获取锁
      //1和2必须是原子的
      //4.加上wait之后最大的意义在于如果没有数据，消费者线程就不必
      //进行空转，节省了资源
      pthread_cond_wait(&cond,&lock);
    }

    int result = data.back();
    data.pop_back();
    printf("result = %d\n", result);
    pthread_mutex_unlock(&lock);
    usleep(123123);
  }
  return NULL;
}


int main(){
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_t tid1, tid2, tid3, tid4;

  pthread_create(&tid1, NULL, Product, NULL);
  pthread_create(&tid2, NULL, Product, NULL);
  pthread_create(&tid3, NULL, Consume, NULL);
  pthread_create(&tid4, NULL, Consume, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);

  return 0;
}
