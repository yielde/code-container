#include <errno.h>
#include <limits.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define BLKSIZE PIPE_BUF
#define MILLION 1000000L
#define D_MILLION 1000000.0

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

static int gettimeout(struct timeval end, struct timeval *timeoutp) {
  gettimeofday(timeoutp, NULL);
  timeoutp->tv_sec = end.tv_sec - timeoutp->tv_sec;
  timeoutp->tv_usec = end.tv_usec - timeoutp->tv_usec;
  if (timeoutp->tv_usec >= MILLION) {
    timeoutp->tv_sec++;
    timeoutp->tv_usec -= MILLION;
  }
  if (timeoutp->tv_usec < 0) {
    timeoutp->tv_sec--;
    timeoutp->tv_usec += MILLION;
  }
  if ((timeoutp->tv_sec < 0) ||
      ((timeoutp->tv_sec == 0) && (timeoutp->tv_usec == 0))) {
    errno = ETIME;
    return -1;
  }
  return 0;
}

struct timeval add2currenttime(double seconds) {
  struct timeval newtime;
  gettimeofday(&newtime, NULL);
  newtime.tv_sec += (int)seconds;
  newtime.tv_usec +=
      (int)((seconds - (int)seconds) * D_MILLION + 0.5);  // 计算微妙部分
  if (newtime.tv_usec >= MILLION) {
    newtime.tv_sec++;
    newtime.tv_usec -= MILLION;
  }
  return newtime;
}

int waitfdtimed(int fd, struct timeval end) {
  fd_set readset;
  int retval;
  struct timeval timeout;
  if ((fd < 0) || (fd >= FD_SETSIZE)) {
    errno = EINVAL;
    return -1;
  }
  FD_ZERO(&readset);
  FD_SET(fd, &readset);
  if (gettimeout(end, &timeout) == -1) return -1;
  while (((retval = select(fd + 1, &readset, NULL, NULL, &timeout)) == -1) &&
         (errno == EINTR)) {
    if (gettimeout(end, &timeout) == -1) return -1;
    FD_ZERO(&readset);
    FD_SET(fd, &readset);
  }
  if (retval == 0) {
    errno = ETIME;
    return -1;
  }
  if (retval == -1) {
    return -1;
  }

  return 0;
}

ssize_t readtimed(int fd, void *buf, size_t nbyte, double seconds) {
  struct timeval timedone;
  timedone = add2currenttime(seconds);
  if (waitfdtimed(fd, timedone) == -1) return (ssize_t)(-1);
  return r_read(fd, buf, nbyte);
}