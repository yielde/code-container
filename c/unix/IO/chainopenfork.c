#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 1024
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define CREATE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char *argv[]) {
  char buf[BUFSIZE];
  pid_t childpid = 0;
  int fd, i;
  if (argc != 3) {
    fprintf(stderr, "Usage: %s processes filename\n", argv[0]);
    return 1;
  }

  fd = open(argv[2], CREATE_FLAGS, CREATE_PERMS);
  if (fd < 0) {
    perror("Failed to open file");
    return 1;
  }
  int n = atoi(argv[1]);

  for (i = 1; i < n; i++) {
    if (childpid = fork()) break;
  }
  if (childpid == -1) {
    perror("Failed to fork");
    return 1;
  }

  sprintf(buf, "i:%d process:%d ", i, getpid());
  write(fd, buf, strlen(buf));
  sleep(1);
  sprintf(buf, "process: %d, parent: %d, child: %d\n", getpid(), getppid(),
          childpid);
  write(fd, buf, strlen(buf));
  return 0;
}