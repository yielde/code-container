#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t childpid;
  pid_t waitreturn;
  if (argc != 2) {
    fprintf(stderr, "message error");
    return 1;
  }
  int n = atoi(argv[1]);
  for (int i = 1; i < n; i++) {
    childpid = fork();
    if (childpid) break;
  }
  while (childpid != (waitreturn = wait(NULL))) {
    if (errno == ECHILD) {
      fprintf(stderr, "Hey, process %d, parent %d\n", getpid(), getppid());
      break;
    } else if ((waitreturn == -1) && (errno != EINTR))
      break;
  }
  fprintf(stderr, "process %d, parent id %d\n", getpid(), getppid());
  return 0;
}