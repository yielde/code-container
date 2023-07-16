#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int setblock(int fd) {
  int fdflags;
  if ((fdflags = fcntl(fd, F_GETLK, 0))) return -1;
  fdflags &= ~O_NONBLOCK;
  if (fcntl(fd, F_SETFL, fdflags) == -1) return -1;
  return 0;
}