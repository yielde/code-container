#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define FIFO_PERM (S_IWUSR | S_IRUSR)

int dofifochild(const char *fifoname, const char *idstring);
int dofifoparent(const char *fifoname);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return 1;
  }
  pid_t childpid;
  if (mkfifo(argv[1], FIFO_PERM) == -1) {
    if (errno != EEXIST) {
      perror("Failed to make FIFO");
      return 1;
    }
  }
  if ((childpid = fork()) == -1) {
    perror("Failed to fork");
    return 1;
  }
  if (childpid == 0) {
    return dofifochild(argv[1], "this was written by child");
  } else {
    return dofifoparent(argv[1]);
  }
}