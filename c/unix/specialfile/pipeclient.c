#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../restart.h"
int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s fifoname\n", argv[0]);
    return 1;
  }
  time_t current_time;
  char requestbuf[PIPE_BUF];
  int fd;
  if ((fd = open(argv[1], O_WRONLY)) == -1) {
    perror("Client failed to open FIFO");
    return 1;
  }
  current_time = time(NULL);
  snprintf(requestbuf, PIPE_BUF, "[%d]:%s", getpid(), ctime(&current_time));
  int len = strlen(requestbuf);
  if (r_write(fd, requestbuf, len) != len) {
    perror("Client failed to write");
    return 1;
  }
  r_close(fd);
  return 0;
}