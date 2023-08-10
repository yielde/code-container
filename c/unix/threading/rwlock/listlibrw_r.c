
#include <errno.h>
#include <pthread.h>

#include "../../mylist/listlib.h"

static pthread_rwlock_t listlock;
static int lockiniterror = 0;
static pthread_once_t lockisinitialized = PTHREAD_ONCE_INIT;

static void ilock() { lockiniterror = pthread_rwlock_init(&listlock, NULL); }

int initialize_r() {
  if (pthread_once(&lockisinitialized, ilock)) lockiniterror = EINVAL;
  return lockiniterror;
}

int accessdata_r() {
  int error;
  int errorkey = 0;
  int key;
  if (error = pthread_rwlock_wrlock(&listlock)) {
    errno = error;
    return -1;
  }
  key = accessdata();
  if (key == -1) {
    errorkey = errno;
    pthread_rwlock_unlock(&listlock);
    errno = errorkey;
    return -1;
  }
  if (error = pthread_rwlock_unlock(&listlock)) {
    errno = error;
    return -1;
  }
  return key;
}

int adddata_r(data_t data) {
  int error;
  if (error = pthread_rwlock_wrlock(&listlock)) {
    errno = error;
    return -1;
  }

  if (adddata(data) == -1) {
    error = errno;
    pthread_rwlock_unlock(&listlock);
    errno = error;
    return -1;
  }

  if (error = pthread_rwlock_unlock(&listlock)) {
    errno = error;
    return -1;
  }
  return 0;
}

int getdata_r(int key, data_t *datap) {
  int error;
  if (error = pthread_rwlock_rdlock(&listlock)) {
    errno = error;
    return -1;
  }
  if (getdata(key, datap) == -1) {
    error = errno;
    pthread_rwlock_unlock(&listlock);
    errno = error;
    return -1;
  }
  if (error = pthread_rwlock_unlock(&listlock)) {
    errno = error;
    return -1;
  }
  return 0;
}

int freekey_r(int key) {
  int error;
  if (error = pthread_rwlock_wrlock(&listlock)) {
    errno = error;
    return -1;
  }
  if (freekey(key) == -1) {
    error = errno;
    pthread_rwlock_unlock(&listlock);
    errno = error;
    return -1;
  }
  if (error = pthread_rwlock_unlock(&listlock)) {
    errno = error;
    return -1;
  }

  return 0;
}
