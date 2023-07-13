#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include "../restart.h"

#define BLKSIZE PIPE_BUF
int readwrite(int fromfd, int tofd) {
  char buf[BLKSIZE];
  fprintf(stderr, "pipe buf size %d", PIPE_BUF);
  int bytesread;
  if ((bytesread = r_read(fromfd, buf, BLKSIZE)) == -1) return -1;
  if (bytesread == 0) return 0;
  if (r_write(tofd, buf, bytesread) == -1) return -1;
  return bytesread;
}

int main() {
  int numbytes;
  numbytes = readwrite(STDIN_FILENO, STDOUT_FILENO);
  fprintf(stderr, "Num of bytes copied: %d\n", numbytes);
  return 0;
}