#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../restart.h"
int main(void) {
  pid_t childpid;
  sigset_t mask, omask;
  if ((sigfillset(&mask) == -1) ||
      (sigprocmask(SIG_SETMASK, &mask, &omask) == -1)) {
    perror("Failed to block the signals");
    return 1;
  }
  if ((childpid = fork()) == -1) {
    perror("Failed to fork child");
    return 1;
  }

  if (childpid == 0) {
    sleep(1);
    execl("/bin/ls", "ls", "-l", "/dev", NULL);
    perror("Child failed to exec");
    return 1;
  }

  if (sigprocmask(SIG_SETMASK, &omask, NULL) == -1) {
    perror("parent failed to restore signal mask");
    return 1;
  }

  if (r_wait(NULL) == -1) {
    perror("Parent failed to wait for child");
    return 1;
  }
  fprintf(stderr, "OVER!\n");
  return 0;
}