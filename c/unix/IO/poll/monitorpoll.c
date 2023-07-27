#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../restart.h"

#define BUFSIZE 1024
void docommand(char *buf, int num) {
  fprintf(stderr, "Got command %.*s", num, buf);
}

void monitorpoll(int fd[], int numfds) {
  char buf[BUFSIZE];
  int numnow = 0;
  int numready;
  int bytesread;
  struct pollfd *pollfd;

  for (int i = 0; i < numfds; i++)
    if (fd[i] > 0) numnow++;
  if ((pollfd = (void *)calloc(numfds, sizeof(struct pollfd))) == NULL) return;
  for (int i = 0; i < numfds; i++) {
    (pollfd + i)->fd = *(fd + 1);
    (pollfd + i)->events = POLLRDNORM;
  }
  while (numnow > 0) {
    numready = poll(pollfd, numfds, -1);
    if ((numready == -1) && (errno == EINTR))
      continue;
    else if (numready == -1)
      break;
    for (int i = 0; i<numfds && & numready> 0; i++) {
      if ((pollfd + i)->revents) {
        if ((pollfd + i)->revents & (POLLRDNORM | POLLIN)) {
          bytesread = r_read(fd[i], buf, BUFSIZE);
          numready--;
          if (bytesread > 0)
            docommand(buf, bytesread);
          else
            bytesread = -1;
        } else if ((pollfd + i)->revents & (POLLERR | POLLHUP))
          bytesread = -1;
        else
          bytesread = 0;
        if (bytesread == -1) {
          r_close(fd[i]);
          (pollfd + i)->fd = -1;
          numnow--;
        }
      }
    }
  }
  for (int i = 0; i < numfds; i++) r_close(fd[i]);
  free(pollfd);
}

int main(int argc, char *argv[]) {
  int *fds;
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s file1 file2 ...\n", argv[0]);
    return 1;
  }
  fds = (int *)malloc((argc - 1) * sizeof(int));
  if (fds == NULL) {
    fprintf(stderr, "Malloc failed\n");
    return 1;
  }
  for (int i = 0; i < argc - 1; i++) {
    fds[i] = open(argv[i + 1], O_RDONLY);
    if (fds[i] == -1) {
      fprintf(stderr, "Error opening %s from read\n", argv[i + 1]);
      return 1;
    }
  }
  monitorpoll(fds, argc - 1);
  return 0;
}