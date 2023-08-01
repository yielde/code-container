#include <errno.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../restart.h"

int main(int argc, char *argv[]) {
  pid_t childpid;
  double clockticks;
  double cticks;
  struct tms tmend;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s command\n", argv[0]);
    return 1;
  }

  if ((childpid = fork()) == -1) {
    perror("Failed to fork");
    return 1;
  }

  if (childpid == 0) {
    execvp(argv[1], &argv[1]);
    perror("Child faild to execvp the command");
    return 1;
  }

  if (r_wait(NULL) == -1) {
    perror("Failed to wait for child");
    return 1;
  }

  if (times(&tmend) == (clock_t)-1) {
    perror("Failed to get end time");
    return 1;
  }

  if ((clockticks = (double)sysconf(_SC_CLK_TCK)) == -1) {
    perror("Failed to determine clock ticks per second");
    return 1;
  }

  if (clockticks == 0) {
    fprintf(stderr, "Invalid number of ticks per second\n");
    return 1;
  }

  cticks =
      tmend.tms_cutime + tmend.tms_cstime - tmend.tms_utime - tmend.tms_stime;

  printf("%s used %ld clock ticks or %f seconds\n", argv[1], (long)cticks,
         cticks / clockticks);
  return 0;
}