#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../restart.h"
int main(int argc, char *argv[]) {
  pid_t childpid;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s n\n", argv[0]);
    return 1;
  }
  int i;
  int n = atoi(argv[1]);
  for (i = 1; i < n; i++) {
    if ((childpid = fork()) <= 0) break;
  }
  while (r_wait(NULL) > 0)
    ;
  fprintf(stderr, "i: %d MYID: %d parent ID: %d\n", i, getpid(), getppid());
}