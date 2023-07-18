
#include "my_atomic_logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s process_num filename", argv[0]);
    return 1;
  }
  pid_t childpid;
  int i;
  char buf[BUFSIZE];
  int n = atoi(argv[1]);
  for (i = 1; i < n; i++) {
    if (childpid = fork()) break;
  }
  if (childpid == -1) {
    perror("Failed to fork");
    return 1;
  }

  if (atomic_log_open(argv[2]) == -1) {
    perror("Failed to open file");
    return 1;
  }

  sprintf(buf, "fork_i: %d, process: %d", i, getpid());
  atomic_log_array(buf, strlen(buf));
  sprintf(buf, " child: %d, parent: %d\n", childpid, getppid());
  atomic_log_string(buf);
  if (childpid == 0) {
    atomic_log_printf("last one i:%d, process: %d, parent: %d\n", i, getpid(),
                      getppid());
  }

  if (atomic_log_send() == -1) {
    perror("Failed to save to log file");
    return 1;
  }

  atomic_log_close();
  return 0;
}