#include <pthread.h>

static int globalerror = 0;
static pthread_mutex_t errorlock = PTHREAD_MUTEX_INITIALIZER;

int geterror(int *error) {
  int terror;
  if (terror = pthread_mutex_lock(&errorlock)) return terror;
  *error = globalerror;
  return pthread_mutex_unlock(&errorlock);
}

int seterror(int error) {
  if (!error) return error;
  int terror;
  if (terror = pthread_mutex_lock(&errorlock)) return terror;
  if (!globalerror) globalerror = error;
  terror = pthread_mutex_unlock(&errorlock);
  return terror ? terror : error;
}