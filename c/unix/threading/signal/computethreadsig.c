#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../mutex/globalerror.h"
#include "signalthread.h"

int main(int argc, char *argv[]) {
  int error;
  int numthreads;
  pthread_t *tids;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s numthread\n", argv[0]);
    return 1;
  }

  if (error = signalthreadinit(SIGUSR1)) {
    fprintf(stderr, "Failed to set up signal thread: %s\n", strerror(error));
    return 1;
  }

  numthreads = atoi(argv[1]);
  if ((tids = (pthread_t *)calloc(numthreads, sizeof(pthread_t))) == NULL) {
    perror("Failed to allocate space for thread IDs");
    return 1;
  }
  for (int i = 0; i < numthreads; i++) {
    if (error = pthread_create(tids + i, NULL, computethread, NULL)) {
      fprintf(stderr, "Failed to join thread: %d: %s\n", i, strerror(error));
      return 1;
    }
  }
  fprintf(stderr, "Send SIGUSR1(%d) signal to proc %ld to stop calculation\n",
          SIGUSR1, (long)getpid());
  for (int i = 0; i < numthreads; i++) {
    if (error = pthread_join(tids[i], NULL)) {
      fprintf(stderr, "Failed to join thread %d: %s\n", i, strerror(error));
      return 1;
    }
  }
  if (showresults()) return 1;
  return 0;
}