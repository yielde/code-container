#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "globalerror.h"

int showresults();

int main(int argc, char *argv[]) {
  int error;
  int i;
  int numthreads;
  int sleeptime;
  pthread_t *tids;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s numthreads sleeptime\n", argv[0]);
    return 1;
  }
  numthreads = atoi(argv[1]);
  sleeptime = atoi(argv[2]);

  if ((tids = (pthread_t *)calloc(numthreads, sizeof(pthread_t))) == NULL) {
    perror("Failed to allocate space for thread IDs");
    return 1;
  }

  for (i = 0; i < numthreads; i++) {
    if (error = pthread_create(tids + i, NULL, computethread, NULL)) {
      fprintf(stderr, "Failed to set done: %s\n", strerror(error));
      return 1;
    }
  }
  sleep(sleeptime);
  if (error = setdone()) {
    fprintf(stderr, "Failed to set done: %s\n", strerror(error));
    return 1;
  }

  for (i = 0; i < numthreads; i++) {
    if (error = pthread_join(tids[i], NULL)) {
      fprintf(stderr, "Failed to join thread %d: %s\n", i, strerror(error));
      return 1;
    }
  }
  if (showresults()) return 1;
  return 0;
}
