#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t childpid;
  childpid = fork();
  if (childpid == -1) {
    perror("Fork failed");
    return -1;
  }
  if (childpid == 0)
    printf("child: %d\n", getpid());
  else
    printf("parent: %d\n", getpid());
}