#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *start_thread(void *arg) {
  printf("thread %d start\n", (int *)arg);
  fflush(stdout);
  sleep(10);
  return NULL;
}

int main() {
  pthread_t thread;
  int ret;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  sleep(15);
  ret = pthread_create(&thread, &attr, start_thread, (void *)1);
  int stat;
  pthread_attr_getdetachstate(&attr, &stat);
  if (stat == PTHREAD_CREATE_DETACHED) {
    printf("pthread detached\n");
  } else if (stat == PTHREAD_CREATE_JOINABLE) {
    printf("pthread joinable\n");
  }
  // pthread_join(thread, NULL);
  sleep(15);
  pthread_create(&thread, &attr, start_thread, (void *)2);
  sleep(120);
  return 0;
}