#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "../../restart.h"
#define BUFSIZE 1024
void docommand(char *buf, int num) {
  fprintf(stderr, "Got command: %.*s", num, buf);
}

void monitorselect(int fd[], int numfds) {
  char buf[BUFSIZE];
  int bytesread;
  int numnow, numready;
  fd_set readset;
  int maxfd = 0;

  for (int i = 0; i < numfds; i++) {
    if ((fd[i] < 0) || (fd[i] >= FD_SETSIZE)) return;
    if (fd[i] >= maxfd) maxfd = fd[i] + 1;
  }
  numnow = numfds;
  while (numnow > 0) {
    FD_ZERO(&readset);
    for (int i = 0; i < numfds; i++)
      if (fd[i] > 0) FD_SET(fd[i], &readset);
    numready = select(maxfd, &readset, NULL, NULL, NULL);
    if ((numready == -1) && (errno == EINTR))
      continue;
    else if (numready == -1)
      break;
    for (int i = 0; (i < numfds) && (numready > 0); i++) {
      if (fd[i] == -1) continue;
      if (FD_ISSET(fd[i], &readset)) {
        bytesread = r_read(fd[i], buf, BUFSIZE);
        numready--;
        if (bytesread > 0)
          docommand(buf, bytesread);
        else {
          r_close(fd[i]);
          fd[i] = -1;
          numnow--;
        }
      }
    }
  }
  for (int i = 0; i < numfds; i++)
    if (fd[i] >= 0) r_close(fd[i]);
}

int main(int argc, char *argv[]) {
  int *fds;
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s pipe1 pipe2 ...\n", argv[0]);
    return 1;
  }
  fds = (int *)malloc((argc - 1) * sizeof(int));
  if (fds == NULL) {
    fprintf(stderr, "Malloc error\n");
    return 1;
  }
  for (int i = 0; i < argc - 1; i++) {
    fds[i] = open(argv[i + 1], O_RDONLY);
    if (fds[i] == -1) {
      fprintf(stderr, "Error openning %s", argv[i + 1]);
      return 1;
    }
  }
  monitorselect(fds, argc - 1);
  return 0;
}