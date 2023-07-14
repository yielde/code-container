#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>

#define BLKSIZE PIPE_BUF

pid_t r_wait(int *stat_local) {
  int retval;
  while ((retval = wait(stat_local)) && (errno == EINVAL))
    ;
  return retval;
}

ssize_t r_read(int fd, void *buf, size_t size) {
  ssize_t retval;
  while (retval = read(fd, buf, size), retval == -1 && errno == EINTR)
    ;
  return retval;
}

ssize_t r_write(int fd, void *buf, size_t size) {
  char *bufp;
  size_t bytestowrite;
  size_t byteswritten;
  size_t totalbytes;

  for (bufp = buf, bytestowrite = size, totalbytes = 0; bytestowrite > 0;
       bufp += byteswritten, bytestowrite -= byteswritten) {
    byteswritten = write(fd, bufp, bytestowrite);
    if ((byteswritten == -1) && (errno == EINTR)) return -1;
    if (byteswritten == -1) byteswritten = 0;
    totalbytes += byteswritten;
  }
  return totalbytes;
}

int r_close(int fd) {
  int retval;
  while (retval = close(fd), retval == -1 && errno == EINTR)
    ;
  return retval;
}

int readwrite(int fromfd, int tofd) {
  char buf[BLKSIZE];
  int bytesread;

  if ((bytesread = r_read(fromfd, buf, BLKSIZE)) < 0) return -1;
  if (bytesread == 0) return 0;
  if (r_write(tofd, buf, bytesread) < 0) return -1;
  return bytesread;
}

int copyfile(int fromfd, int tofd) {
  int bytesread;
  int totalbytes = 0;

  while ((bytesread = readwrite(fromfd, tofd)) > 0) {
    totalbytes += bytesread;
  }
  return totalbytes;
}