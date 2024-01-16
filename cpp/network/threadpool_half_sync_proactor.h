#ifndef THREADPOOL_HALF_SYNC_PROACTOR_H
#define THREADPOOL_HALF_SYNC_PROACTOR_H

#include <pthread.h>

#include <cstdio>
#include <exception>
#include <iostream>
#include <list>

#include "locker.h"
using namespace std;
template <typename T>
class threadpool {
 public:
  threadpool(int threadnumber = 8, int max_requests = 10000);
  ~threadpool();
  bool append(T *request);

 private:
  static void *worker(void *arg);
  void run();
  int m_thread_number;
  int m_max_requests;
  pthread_t *m_threads;
  std::list<T *> m_workqueue;
  locker m_ququelocker;
  sem m_queuestat;
  bool m_stop;
};

template <typename T>
threadpool<T>::threadpool(int thread_number, int max_requests)
    : m_thread_number(thread_number),
      m_max_requests(max_requests),
      m_stop(false),
      m_threads(NULL) {
  if ((thread_number <= 0) || (max_requests <= 0)) {
    throw std::exception();
  }
  m_threads = new pthread_t[m_thread_number];
  if (!m_threads) {
    throw std::exception();
  }
  for (int i = 0; i < thread_number; ++i) {
    cout << "create " << i << " thread" << endl;
    if (pthread_create(m_threads + i, NULL, worker, this) != 0) {
      delete[] m_threads;
      throw std::exception();
    }
  }
}

template <typename T>
threadpool<T>::~threadpool() {
  delete[] m_threads;
  m_stop = true;
}

template <typename T>
bool threadpool<T>::append(T *request) {
  m_ququelocker.lock();
  if (m_workqueue.size() > m_max_requests) {
    m_ququelocker.unlock();
    return false;
  }
  m_workqueue.push_back(request);
  m_ququelocker.unlock();
  m_queuestat.post();
  return true;
}

template <typename T>
void *threadpool<T>::worker(void *arg) {
  threadpool *pool = (threadpool *)arg;
  pool->run();
  return pool;
}

template <typename T>
void threadpool<T>::run() {
  while (!m_stop) {
    m_queuestat.wait();
    m_ququelocker.lock();
    if (m_workqueue.empty()) {
      m_ququelocker.unlock();
      continue;
    }
    T *request = m_workqueue.front();
    m_workqueue.pop_front();
    m_ququelocker.unlock();
    if (!request) {
      continue;
    }
    request->process();
  }
}
#endif