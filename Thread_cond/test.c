#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 2
pthread_cond_t cond;
pthread_mutex_t mutex;

void* ThreadEntry1(void* arg){
  (void) arg;
  while(1){
    printf("传球\n");
    pthread_cond_signal(&cond);
    sleep(2);
  }
  return NULL;
}

void* ThreadEntry2(void* arg){
  (void) arg;
  while(1){
    pthread_cond_wait(&cond, &mutex);
    printf("扣篮\n");
    sleep(1);
  }

  return NULL;
}

int main(){
  pthread_t tid[THREAD_NUM];
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  
  pthread_create(&tid[0], NULL, ThreadEntry1, NULL);
  pthread_create(&tid[1], NULL, ThreadEntry2, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  
  return 0;
}
