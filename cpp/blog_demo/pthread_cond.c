#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char buf[8] = {0};

void *handler_hello(void *arg) {
  for (;;) {
    sleep(2);
    pthread_mutex_lock(&mutex);
    sprintf(buf, "%s", "hello !");
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
  }

  return NULL;
}

void *handler_print(void *arg) {
  for (;;) {
    pthread_mutex_lock(&mutex);
    while (buf[0] == 0) {
      pthread_cond_wait(&cond, &mutex);
    }
    fprintf(stderr, "%s", buf);
    memset(buf, '\0', 8);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, handler_hello, NULL);
  pthread_create(&tid2, NULL, handler_print, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("%s", buf);
  return 0;
}