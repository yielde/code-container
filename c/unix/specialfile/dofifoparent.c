#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../restart.h"

#define BUFSIZE 256

int dofifoparent(const char *fifoname) {
  int fd;
  int retval;
  char buf[BUFSIZE];
  fprintf(stderr, "[%d]:(parent) about to open FIFO %s...\n", getpid(),
          fifoname);
  if ((fd = open(fifoname, O_RDONLY)) == -1 && (errno != EINTR))
    ;
  if (fd == -1) {
    fprintf(stderr, "[%d]:(parent) failed to open %s\n", getpid(), fifoname);
    return 1;
  }

  if ((retval = r_read(fd, buf, BUFSIZE)) == -1) {
    fprintf(stderr, "[%d]:(parent) failed to read from pipe: %s\n", getpid(),
            strerror(errno));
    return 1;
  }
  fprintf(stderr, "[%d]:(parent) read %s\n", getpid(), buf);
  return 0;
}