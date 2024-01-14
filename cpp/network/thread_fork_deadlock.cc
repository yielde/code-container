#include <pthread.h>
#include <unistd.h>
#include <wait.h>

#include <iostream>

using namespace std;
pthread_mutex_t mutex;

void *another(void *arg) {
  cout << "in child thread, lock the mutex" << endl;
  pthread_mutex_lock(&mutex);
  sleep(5);
  cout << "in child thread, unlock the mutex" << endl;
  pthread_mutex_unlock(&mutex);
}

void prepare() { pthread_mutex_lock(&mutex); }
void infork() { pthread_mutex_unlock(&mutex); }

int main() {
  pthread_mutex_init(&mutex, NULL);
  pthread_t id;
  pthread_create(&id, NULL, another, NULL);
  sleep(1);
  // int pid = fork();
  // pthread_atfork，清除锁状态
  int pid = pthread_atfork(prepare, infork, infork);
  if (pid < 0) {
    pthread_join(id, NULL);
    pthread_mutex_destroy(&mutex);
    return 1;
  } else if (pid == 0) {
    cout << "child: want to get lock" << endl;
    pthread_mutex_lock(&mutex);
    cout << "here can't run" << endl;
    pthread_mutex_unlock(&mutex);
    exit(0);
  } else {
    wait(NULL);
  }
  pthread_join(id, NULL);
  pthread_mutex_destroy(&mutex);
  return 0;
}