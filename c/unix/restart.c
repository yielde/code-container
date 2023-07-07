#include <errno.h>
#include <sys/wait.h>

pid_t r_wait(int *stat_local) {
  int retval;
  while ((retval = wait(stat_local)) && (errno == EINVAL))
    ;
  return retval;
}