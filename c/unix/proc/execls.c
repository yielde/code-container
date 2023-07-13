#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t childpid;
  if ((childpid = fork()) == -1) {
    perror("Failed to fork");
    return 1;
  }
  if (childpid == 0) {
    execl("/bin/ls", "ls", "-al", NULL);
    perror("Child failed to execl ls");
    return 1;
  }
  if (childpid != wait(NULL)) {
    perror("Parent failed to wait due to signal or error");
    return 1;
  }

  return 0;
}