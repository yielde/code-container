#include <aio.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include "../restart.h"

static struct aiocb aiocb;
static sig_atomic_t doneflag;
static int fdout;
static int globalerror;
static int totalbytes;

static void seterror(int error) {
  if (!globalerror) globalerror = error;
  doneflag = 1;
}

static int readstart() {
  int error;
  if (error == aio_read(&aiocb)) seterror(error);
  return error;
}

static void aiohandler(int signo, siginfo_t *info, void *context) {
  int myerrno;
  int mystatus;
  int serrno;

  serrno = errno;
  myerrno = aio_error(&aiocb);
  if (myerrno == EINPROGRESS) {
    errno = serrno;
    return;
  }
  if (myerrno) {
    seterror(myerrno);
    errno = serrno;
    return;
  }

  mystatus = aio_return(&aiocb);
  totalbytes += mystatus;
  aiocb.aio_offset += mystatus;
  fprintf(stderr, "mystatus %d\n", mystatus);
  if (mystatus == 0) {
    doneflag = 1;
  } else if (r_write(fdout, (char *)aiocb.aio_buf, mystatus) == -1) {
    seterror(errno);
  } else if (readstart() == -1)
    seterror(errno);
  errno = serrno;
}

int initsignal(int signo) {
  struct sigaction newact;
  newact.sa_flags = SA_SIGINFO;
  newact.sa_sigaction = aiohandler;

  if ((sigemptyset(&newact.sa_mask) == -1) ||
      (sigaction(signo, &newact, NULL) == -1)) {
    return -1;
  }
  return 0;
}

int initread(int fdread, int fdwrite, int signo, char *buf, int bufsize) {
  aiocb.aio_fildes = fdread;
  aiocb.aio_offset = 0;
  aiocb.aio_buf = (void *)buf;
  aiocb.aio_nbytes = bufsize;
  aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb.aio_sigevent.sigev_signo = signo;
  aiocb.aio_sigevent.sigev_value.sival_ptr = &aiocb;
  fdout = fdwrite;
  doneflag = 0;
  globalerror = 0;
  totalbytes = 0;
  return readstart();
}

int getdone() { return doneflag; }

int geterror() {
  if (doneflag) return globalerror;
  errno = EINVAL;
  return errno;
}

int getbytes() {
  if (doneflag) return totalbytes;
  errno = EINVAL;
  return -1;
}