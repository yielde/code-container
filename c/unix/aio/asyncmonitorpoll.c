#include "asyncmonitorpoll.h"

#include <aio.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#define BLKSIZE 1024

typedef struct {
  char buf[BLKSIZE];
  ssize_t bytes;
  struct aiocb control;
  int doneflag;
  int startedflag;
} aio_t;

static aio_t iops[NUMOPS];

int initaio(int fd, int handle) {
  if (handle >= NUMOPS) {
    errno = EINVAL;
    return -1;
  }

  iops[handle].control.aio_fildes = fd;
  iops[handle].control.aio_offset = 0;
  iops[handle].control.aio_buf = (void *)iops[handle].buf;
  iops[handle].control.aio_nbytes = BLKSIZE;
  iops[handle].control.aio_sigevent.sigev_notify = SIGEV_NONE;
  iops[handle].doneflag = 0;
  iops[handle].startedflag = 0;
  iops[handle].bytes = 0;
  return 0;
}

int readcheck(int handle, char **bufp) {
  int error;
  ssize_t numbytes;
  struct aiocb *thisp;
  thisp = &(iops[handle].control);
  if (iops[handle].doneflag) {
    numbytes = iops[handle].bytes;
    *bufp = (char *)iops[handle].control.aio_buf;
    return numbytes;
  }
  error = aio_error(thisp);
  if (error) {
    errno = error;
    return -1;
  }
  numbytes = aio_return(thisp);
  iops[handle].bytes = numbytes;
  *bufp = (char *)iops[handle].control.aio_buf;
  iops[handle].control.aio_offset += numbytes;
  iops[handle].doneflag = 1;
  return numbytes;
}

int readstart(int handle) {
  int error;
  struct aiocb *thisp;
  thisp = &(iops[handle].control);
  if (iops[handle].startedflag) {
    errno = EINVAL;
    return -1;
  }
  if ((error = aio_read(thisp)) == -1) {
    errno = error;
    return -1;
  }
  iops[handle].startedflag = 1;
  return 0;
}

void reinit(int handle) {
  iops[handle].doneflag = 0;
  iops[handle].startedflag = 0;
  iops[handle].bytes = 0;
}