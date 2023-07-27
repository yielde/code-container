#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int i;
  sigset_t intmask;
  int repeatfactor;
  double y = 0.0;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s repeatfactor\n", argv[0]);
    return 1;
  }

  repeatfactor = atoi(argv[1]);
  if ((sigemptyset(&intmask) == -1) || (sigaddset(&intmask, SIGINT) == -1)) {
    perror("Failed to initialize the signal mask");
    return 1;
  }

  for (;;) {
    if (sigprocmask(SIG_BLOCK, &intmask, NULL) == -1) break;
    fprintf(stderr, "SIGINT signal blocked\n");
    for (i = 0; i < repeatfactor; i++) y += sin((double)i);
    sleep(5);
    fprintf(stderr, "Blocked calculation is finished, y = %f \n", y);
    if (sigprocmask(SIG_UNBLOCK, &intmask, NULL) == -1) break;
    fprintf(stderr, "SIGINT signal unblocked\n");
    sleep(5);
    for (i = 0; i < repeatfactor; i++) y += sin((double)i);
    fprintf(stderr, "Unblocked caculation is finished, y=%f\n", y);
  }
  perror("Failed to change signal mask");
  return 1;
}