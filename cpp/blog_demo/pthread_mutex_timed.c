#define _DEFAULT_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

char* now_time(char buf[]) {
  struct timespec abstime;
  abstime.tv_sec = time(0);
  strftime(buf, 1024, "%r", localtime(&abstime.tv_sec));
  return buf;
}

int main() {
  char buf[1024];
  pthread_mutex_t mutex;
  struct timespec abstime;
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_lock(&mutex);
  char* now = now_time(buf);
  printf("mutex locked, now: %s\n", buf);
  // 设置超时的绝对时间，不设置tv_nsec会返回22，EINVAL
  abstime.tv_sec = time(0) + 10;
  abstime.tv_nsec = 0;
  int ret = pthread_mutex_timedlock(&mutex, &abstime);
  fprintf(stderr, "error %d\n", ret);
  if (ret == ETIMEDOUT) {
    printf("lock mutex timeout\n");
  } else if (ret == 0) {
    printf("lock mutex successfully\n");
  } else if (ret == EINVAL) {
    printf("timedlock param invalid!\n");
  } else {
    printf("other error\n");
  }
  pthread_mutex_unlock(&mutex);
  memset(buf, '\0', 1024);
  now = now_time(buf);
  printf("mutex unlocked, now: %s\n", buf);
  pthread_mutex_destroy(&mutex);
  return 0;
}