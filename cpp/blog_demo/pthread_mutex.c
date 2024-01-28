/*
  5个线程对total加1执行指定次数
*/

#define _DEFAULT_SOURCE 1  // 处理vscode 未定义 pthread_rwlock_t
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 5

int total = 0;                                      // 最终和
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化互斥量
pthread_rwlock_t rwlock;                            // 读写锁变量
typedef struct param {                              // 线程参数类型
  int count;
  int id;
} param;

void *handler(void *arg) {
  struct param *pa = (struct param *)arg;
  pthread_rwlock_rdlock(&rwlock);  // 当主线程不unlock写锁时，会阻塞在这里
  for (int i = 0; i < pa->count; ++i) {
    pthread_mutex_lock(&mutex);  // 加互斥锁
    ++total;
    pthread_mutex_unlock(&mutex);
  }
  pthread_rwlock_unlock(&rwlock);
  printf("thread %d complete\n", pa->id);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s per_thread_loop_count\n", argv[0]);
    return 1;
  }
  // 设置mutex为ADAPTER模式
  pthread_mutexattr_t mutexattr;
  pthread_mutexattr_init(&mutexattr);
  pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ADAPTIVE_NP);
  // 给handler传参
  int loop_count = atoi(argv[1]);
  // 存放线程id的数组
  pthread_t tid[THREAD_COUNT];
  param pa[THREAD_COUNT];

  pthread_rwlock_init(&rwlock, NULL);  // 动态初始化读写锁
  pthread_rwlock_wrlock(&rwlock);  // 给写加锁，等所有线程创建好后解锁，线程执行
  for (int i = 0; i < THREAD_COUNT; ++i) {  // 创建5个线程
    pa[i].count = loop_count;
    pa[i].id = i;
    pthread_create(&tid[i], NULL, handler, &pa[i]);
  }

  pthread_rwlock_unlock(&rwlock);
  for (int i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(tid[i], NULL);
  }
  pthread_rwlock_destroy(&rwlock);
  printf("thread count: %d\n", THREAD_COUNT);
  printf("per thread loop count: %d\n", loop_count);
  printf("total except: %d\n", loop_count * 5);
  printf("total result: %d\n", total);

  int kind;
  pthread_mutexattr_gettype(&mutexattr, &kind);
  if (kind == PTHREAD_MUTEX_ADAPTIVE_NP) {
    printf("mutex type is %s", "PTHREAD_MUTEX_ADAPTIVE_NP\n");
  }
  return 0;
}