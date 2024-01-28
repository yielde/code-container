#define _DEFAULT_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define COUNT 10
#define THR_NUM 4

pthread_barrier_t barrier;
long total_arr[THR_NUM] = {0};

void *handler(void *arg) {
  long idx = (long)arg;
  long tmp = 0;
  for (int i = 0; i < COUNT; ++i) {
    ++tmp;
    sleep(1);
  }
  total_arr[idx] = tmp;
  printf("thread %ld complete, count %ld\n", idx, tmp);
  pthread_barrier_wait(&barrier);
  return NULL;
}

int main() {
  pthread_t tids[THR_NUM];
  unsigned long total = 0;

  pthread_barrier_init(&barrier, NULL, THR_NUM + 1);  // 包含主线程
  for (long i = 0; i < THR_NUM; ++i) {
    pthread_create(&tids[i], NULL, handler, (void *)i);
  }
  pthread_barrier_wait(&barrier);
  for (int i = 0; i < THR_NUM; ++i) {
    total += total_arr[i];
  }

  for (int i = 0; i < THR_NUM; ++i) {
    pthread_join(tids[i], NULL);
  }
  pthread_barrier_destroy(&barrier);
  printf("total: %lu\n", total);
}