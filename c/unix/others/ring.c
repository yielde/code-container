#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fd[2];
  pid_t childpid;
  int nprocess;
  int error;
  int i;

  if ((argc != 2) || ((nprocess = atoi(argv[1])) < 1)) {
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
    return 1;
  }
  if (pipe(fd) == -1) {
    perror("Failed to starting pipe");
    return 1;
  }
  if ((dup2(fd[0], STDIN_FILENO)) == -1 ||
      ((dup2(fd[1], STDOUT_FILENO)) == -1)) {
    perror("Failed to connect pipe");
    return 1;
  }
  if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
    perror("Failed to close extra descriptors");
    return 1;
  }

  for (i = 1; i < nprocess; i++) {
    if (pipe(fd) == -1) {
      fprintf(stderr, "[%d]: failed to create pipe %d: %s\n", getpid(), i,
              strerror(errno));
      return 1;
    }

    if ((childpid = fork()) == -1) {
      fprintf(stderr, "[%d]: failed to create child %d: %s\n", getpid(), i,
              strerror(errno));
      return 1;
    }

    if (childpid) {
      error = dup2(fd[0], STDIN_FILENO);
    } else {
      error = dup2(fd[1], STDOUT_FILENO);
    }
    if (error == -1) {
      fprintf(stderr, "[%d]: failed to dup pipes for iteration %d: %s\n",
              getpid(), i, strerror(errno));
      return 1;
    }
    if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
      fprintf(stderr, "[%d]: failed to create child %d: %s\n", getpid(), i,
              strerror(errno));
      return 1;
    }
    if (childpid) break;
  }

  wait(NULL);

  fprintf(stderr, "This is process %d with ID %d and parent id %d\n", i,
          getpid(), getppid());
  return 0;
}