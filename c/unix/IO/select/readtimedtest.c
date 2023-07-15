#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "../../restart.h"

#define BUFSIZE 1000
#define MSG "Got Ctrl-C\n"
#define MSGSIZE sizeof(MSG)

static void handler(int signo) { r_write(STDOUT_FILENO, MSG, MSGSIZE); }

int main() {
  char buf[BUFSIZE];
  struct sigaction act;
  int num;
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  if (sigaction(SIGINT, &act, NULL) == -1) {
    fprintf(stderr, "Error setting up signal handler\n");
    return 1;
  }

  for (int i = 0; i < 5; i++) {
    num = readtimed(STDIN_FILENO, buf, BUFSIZ, 10.0);
    if (num == 0) continue;
    if (num < 0) {
      perror("readtimed error");
      break;
    }
    fprintf(stderr, "Got %d bytes\n", num);
  }
  return 0;
}