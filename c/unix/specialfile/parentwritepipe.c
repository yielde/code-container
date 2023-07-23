/*
 * @Author: Hui Tong
 * @Date: 2023-07-22 11:03:38
 * @LastEditTime: 2023-07-22 11:32:58
 * @Description:
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 10

int main() {
  char bufin[BUFSIZ] = "empty";
  char bufout[] = "hello";
  int bytesin;
  pid_t childpid;
  int fd[2];
  if (pipe(fd) == -1) {
    perror("Failed to create the pipe");
    return 1;
  }
  bytesin = strlen(bufin);
  childpid = fork();
  if (childpid == -1) {
    perror("Failed to fork");
    return 1;
  }
  if (childpid) {
    write(fd[1], bufout, strlen(bufout) + 1);
  } else {
    bytesin = read(fd[0], bufin, BUFSIZ);
  }

  fprintf(stderr, "[%d]: my bufin is (%.*s), mybufout is (%s)\n", getpid(),
          bytesin, bufin, bufout);
}