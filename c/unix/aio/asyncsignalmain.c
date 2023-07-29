#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "asyncmonitorsignal.h"

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BLKSIZE 1024

void dowork() {
  fprintf(stderr, "Hello!\n");
  sleep(3);
};

int main(int argc, char *argv[]) {
  char buf[BLKSIZE];
  int error;
  int fd1;
  int fd2;
  int done = 0;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s filename1 filename2\n", argv[0]);
    return 1;
  }

  if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
    return 1;
  }

  if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, MODE)) == -1) {
    fprintf(stderr, "Failed to open %s: %s\n", argv[2], strerror(errno));
    return 1;
  }

  if (initsignal(SIGRTMAX) == -1) {
    perror("Failed to inite the first read");
    return 1;
  }

  if (initread(fd1, fd2, SIGRTMAX, buf, BLKSIZE) == -1) {
    perror("Failed to initate the first read");
    return 1;
  }

  for (;;) {
    dowork();
    if (!done)
      if (done = getdone())
        if (error = geterror())
          fprintf(stderr, "Failed to copy file: %s\n", strerror(error));
        else
          fprintf(stderr, "Copy successful, %d bytes\n", getbytes());
  }
}