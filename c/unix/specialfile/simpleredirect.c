/*
 * @Author: Hui Tong
 * @Date: 2023-07-22 23:26:02
 * @LastEditTime: 2023-07-22 23:56:22
 * @Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
  pid_t childpid;
  int fd[2];
  if ((pipe(fd) == -1) || ((childpid = fork()) == -1)) {
    perror("Failed to setup pipeline");
    return 1;
  }
  if (childpid == 0) {
    if (dup2(fd[1], STDOUT_FILENO) == -1)
      perror("Failed to redirect stdout of ls");
    if ((close(fd[0]) == -1) || (close(fd[1])) == -1)
      perror("Failed to close extra pipe descriptors on ls");
    else {
      execl("/bin/ls", "ls", "-l", NULL);
      perror("Failed to exec ls");
    }
    return 1;
  }
  if (dup2(fd[0], STDIN_FILENO) == -1)
    perror("Failed to redirect stdin of sort");
  if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
    perror("Failed to close extra pipe descriptors on sort");
  else {
    execl("/usr/bin/sort", "sort", "-n", "+4", NULL);
    perror("Failed to exec sort");
  }
  return 1;
}