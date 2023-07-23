/*
 * @Author: Hui Tong
 * @Date: 2023-07-22 12:13:23
 * @LastEditTime: 2023-07-22 13:17:37
 * @Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../restart.h"

int main(int argc, char *argv[]) {
  char buf[] = "g";
  pid_t childpid = 0;
  int fd[2];
  int i, n;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s processes\n", argv[1]);
    return 1;
  }
  n = atoi(argv[1]);
  if (pipe(fd) == -1) {
    perror("Failed to create the synchronization pipe");
    return 1;
  }
  for (i = 1; i < n; i++) {
    if ((childpid = fork()) <= 0) break;
  }
  if (childpid > 0) {
    for (i = 0; i < n; i++) {
      if (r_write(fd[1], buf, 1) != 1)
        perror("Failed to write synchronization characters");
    }
  }
  if (r_read(fd[0], buf, 1) != 1)
    perror("Failed to read synchronization characters");
  fprintf(stderr, "i:%d process ID:%d parent ID: %d childID:%d\n", i, getpid(),
          getppid(), childpid);
  return (childpid == -1);
}