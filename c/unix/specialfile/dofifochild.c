#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../restart.h"
#define BUFSIZE 256

int dofifochild(const char *fifoname, const char *idstring) {
  int fd;
  int retval;
  char buf[BUFSIZE];
  ssize_t strsize;
  fprintf(stderr, "[%d]:(child) about to open FIFO %s...\n", getpid(),
          fifoname);
  while (((fd = open(fifoname, O_WRONLY)) == -1) && (errno == EINTR))
    ;
  if (fd == -1) {
    fprintf(stderr, "[%d]:(child) failed to open named pipe %s for write: %s\n",
            getpid(), fifoname, strerror(errno));
    return 1;
  }
  retval = snprintf(buf, BUFSIZE, "[%d]:%s\n", getpid(), idstring);
  if (retval < 0) {
    fprintf(stderr, "[%d]:(child) failed to make the string:\n", getpid());
    return 1;
  }
  strsize = strlen(buf) + 1;
  fprintf(stderr, "[%d]:(child) about to write...\n", getpid());
  retval = r_write(fd, buf, strsize);
  if (retval != strsize) {
    fprintf(stderr, "[%d]:(child) failed to write to pipe: %s\n", getpid(),
            strerror(errno));
    return 1;
  }
  fprintf(stderr, "[%d]:(child) finishing...\n", getpid());
  return 0;
}