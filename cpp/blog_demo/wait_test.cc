#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
pid_t r_wait(int *stat) {
  int ret;
  while (((ret = wait(stat)) == -1) && (errno == EINTR))
    ;
  return ret;
}
int main() {
  int stat;
  pid_t pid = fork();
  if (pid > 0) {
    pid_t child_pid;
    int ret;
    while ((ret = r_wait(&stat)) != -1) {
      printf("parent: child pid %d exit with code %d\n", ret,
             WEXITSTATUS(stat));  // 获取正常退出的子进程的返回值
      continue;
    }
    if (errno == ECHILD) {
      printf("parent: no child need to wait\n");
    }
  } else if (pid == 0) {
    pid_t child_pid = getpid();
    sleep(3);
    printf("child: i'm child, pid: %d\n", child_pid);
    exit(10);
  } else {
    printf("fork failure\n");
  }
  return 0;
}