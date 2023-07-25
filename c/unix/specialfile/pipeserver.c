#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../restart.h"

#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH)

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s fifoname > logfile\n", argv[0]);
    return 1;
  }
  int fd;
  if ((mkfifo(argv[1], FIFO_PERMS) == -1) && (errno != EEXIST)) {
    perror("Failed to create a FIFO");
    return 1;
  }
  if ((fd = open(argv[1], O_RDWR)) == -1) {
    perror("Failed to open FIFO");
    return 1;
  }
  copyfile(fd, STDOUT_FILENO);
  return 1;
}