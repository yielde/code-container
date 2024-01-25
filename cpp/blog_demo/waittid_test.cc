#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
int main() {
  int stat;
  pid_t pid = fork();
  if (pid > 0) {
    siginfo_t info;
    int ret;
    memset(&info, '\0', sizeof(info));
    ret = waitid(P_PGID, getpid(), &info, WEXITED | WNOWAIT);
    if ((ret == 0) && (info.si_pid == pid)) {
      printf("child %d exit, exit event: %d, exit status: %d\n", pid,
             info.si_code, info.si_status);
    }
  } else if (pid == 0) {
    sleep(3);
    printf("i'm child, pid: %d\n", getpid());
    return 10;
  } else {
    printf("fork failure\n");
  }
  sleep(15);
  return 0;
}