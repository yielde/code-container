#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFF_SIZE 1024
void clean_up(void *arg) { printf("cleanup: %s\n", (char *)arg); }

void *test_func1(void *arg) {
  pthread_t thread = pthread_self();
  printf("thread1: %lu start\n", thread);
  // 构造push handler函数信息
  char s1[BUFF_SIZE];
  char s2[BUFF_SIZE];
  sprintf(s1, "thread1: %lu first handler", thread);
  sprintf(s2, "thread1: %lu second handler", thread);

  pthread_cleanup_push(clean_up, s1);
  pthread_cleanup_push(clean_up, s2);
  // return方式退出
  return ((void *)10);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
}

void *test_func2(void *arg) {
  pthread_t thread = pthread_self();
  printf("thread2: %lu start\n", thread);
  // 构造push handler函数信息
  char s1[BUFF_SIZE];
  char s2[BUFF_SIZE];
  sprintf(s1, "thread2: %lu first handler", thread);
  sprintf(s2, "thread2: %lu second handler", thread);

  pthread_cleanup_push(clean_up, s1);
  pthread_cleanup_push(clean_up, s2);
  // pthread_exit()退出
  pthread_exit((void *)15);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
}

int main() {
  int err;
  pthread_t tid1, tid2;
  void *ret_info;

  // 创建线程1
  err = pthread_create(&tid1, NULL, test_func1, (void *)NULL);
  if (err != 0) {
    printf("create thread failure\n");
  }
  // 后面省略错误判断。。。
  pthread_create(&tid2, NULL, test_func2, (void *)NULL);
  // 接收进程1的退出信息
  pthread_join(tid1, &ret_info);
  printf("thread %lu exit code %d\n", tid1, (int)ret_info);
  // 接收进程2的退出信息
  pthread_join(tid2, &ret_info);
  printf("thread %lu exit code %d\n", tid1, (int)ret_info);

  return 0;
}