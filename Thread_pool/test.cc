#if 0
#include "BlockingQueue.hpp"

BlockingQueue<int> queue(100);

void* Product(void* arg){
  (void) arg;
  int count = 0;
  while(1){
    queue.Push(++count);
    usleep(789789);
  }
  return NULL;
}

void* Consume(void* arg){
  (void) arg;
  while(1){
    int count = 0;
    queue.Pop(&count);
    printf("count = %d\n",count);
    usleep(123123);
  }

  return NULL;
}

//进程间通信的管道本质上就是一个BlockingQueue
int main(){
  pthread_t tid1, tid2;
  
  pthread_create(&tid1, NULL, Product, NULL);
  pthread_create(&tid2, NULL, Consume, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  return 0;
}
#endif

#if 1
#include "threadpool.hpp"

//这个类由用户自定制
//需要依赖哪些数据都可以随意添加和修改
class MyTask : public Task {
public:
  MyTask(int id)
    :id_(id)
  {

  }
  void Run(){
    //执行用户自定制的逻辑
    printf("id_ = %d\n", id_);
  }
private:
  int id_;
};

int main(){
  
  ThreadPool pool(10);
  int i = 0;
  for(; i < 20; ++i){
    pool.AddTask(new MyTask(i));
    usleep(789789);
  }

  return 0;
}
#endif
