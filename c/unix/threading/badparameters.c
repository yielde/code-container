#include <error.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUMTHREADS 10

static void *printarg(void *arg) {
  sleep(1);
  fprintf(stderr, "Thread received %d\n", *(int *)arg);
  return NULL;
}

int main() {
  int error;
  int i;
  pthread_t tid[NUMTHREADS];
  for (i = 0; i < NUMTHREADS; i++) {
    if (error = pthread_create(tid + i, NULL, printarg, (void *)&i)) {
      fprintf(stderr, "Failed to create thread: %d:%s", i, strerror(error));
      tid[i] = pthread_self();
    }
  }
  for (i = 0; i < NUMTHREADS; i++) {
    if (pthread_equal(pthread_self(), tid[i])) continue;
    if (error = pthread_join(tid[i], NULL))
      fprintf(stderr, "Failed to join thread: %d:%s", i, strerror(error));
  }
  printf("All threads done\n");
  return 0;
}