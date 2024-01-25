#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  // int ret = system("lss -l"); //执行错误的命令
  // int ret = system("ls -l"); // 正常执行命令
  int ret = system("sleep 50"); // 执行命令进程被信号杀死
  if (ret == -1) {
    printf("system return -1, errno is: %s", strerror(errno));
  } else if (WIFEXITED(ret) && WEXITSTATUS(ret) == 127) {
    // WIFEXITED(wstatus) returns true if the child terminated normally(在 man wait中)
    // WEXITSTATUS(wstatus) returns the exit status of the child
    printf("shell can't exec the command\n");
  } else {
    if(WIFEXITED(ret)){
      printf("normal termination, exit code = %d\n", WEXITSTATUS(ret));
    }else if(WIFSIGNALED(ret)){
      // WIFSIGNALED(wstatus) returns true if the child process was terminated by a signal.
      printf("abnormal termination, signal number = %d\n", WTERMSIG(ret));
    }
  }
}