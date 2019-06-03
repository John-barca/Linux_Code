#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 2

int g_count = 0;

pthread_mutex_t mut;

void* ThreadEntry(void* arg){
  (void) arg;
  int i = 0;
  pthread_mutex_lock(&mut);
  for(; i < 500000; i++){
    ++g_count;
  }
  pthread_mutex_unlock(&mut);
  return NULL;
}

int main(){
  pthread_t tid[THREAD_NUM];
  int i = 0;
  for(; i < THREAD_NUM; i++){
    pthread_create(&tid[i], NULL, ThreadEntry, NULL);
  }

  for(i = 0; i < THREAD_NUM; i++){
    pthread_join(tid[i],NULL);
  }

  printf("g_count number is --->%d\n",g_count);

  return 0;
}
