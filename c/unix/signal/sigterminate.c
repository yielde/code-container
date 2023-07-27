#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static volatile sig_atomic_t doneflag = 0;

static void setdoneflag(int signo) { doneflag = 1; }

int main() {
  struct sigaction act;
  int count;
  double sum = 0;
  double x;

  act.sa_flags = 0;
  act.sa_handler = setdoneflag;

  if ((sigemptyset(&act.sa_mask) == -1) ||
      (sigaction(SIGINT, &act, NULL) == -1)) {
    perror("Failed to set SIGINT handler");
    return 1;
  }

  while (!doneflag) {
    x = (rand() + 0.5) / (RAND_MAX + 1.0);
    sum += sin(x);
    count++;
    printf("Count is %d and average is %f\n", count, sum / count);
  }

  printf("My program terminating ... \n");
  if (count == 0)
    printf("No values calculated yet\n");
  else
    printf("Count is %d and average is %f\n", count, sum / count);

  return 0;
}