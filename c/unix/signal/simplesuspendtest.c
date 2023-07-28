#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int initsuspend(int signo);
int restore();
int simplesuspend();

int main() {
  fprintf(stderr, "This is process %d\n", getpid());
  for (;;) {
    if (initsuspend(SIGUSR1)) {
      perror("Failed to setup handler for SIGUSER1");
      return 1;
    }
    fprintf(stderr, "Waiting for signal\n");
    if (simplesuspend()) {
      perror("Failed to suspend for signal");
      return 1;
    }
    fprintf(stderr, "Got signal\n");
    if (restore()) {
      perror("Failed to restore original handler");
      return 1;
    }
  }
  return 1;
}