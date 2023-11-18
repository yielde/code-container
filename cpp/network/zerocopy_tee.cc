#include <libgen.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "usage: " << basename(argv[0]) << "filename" << endl;
    return 1;
  }
  int filefd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
  assert(filefd > 0);
  int pipefd_terminal[2];
  int ret = pipe(pipefd_terminal);
  assert(ret != -1);
  int pipefd_file[2];
  ret = pipe(pipefd_file);
  assert(ret != -1);
  // stdin -> 终端管道输入
  ret = splice(STDIN_FILENO, NULL, pipefd_terminal[1], NULL, 32768, 0);
  assert(ret != -1);
  // 终端管道读出 复制到 文件管道输入
  ret = tee(pipefd_terminal[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK);
  assert(ret != -1);
  // 终端管道读出 -> 标准输出
  ret = splice(pipefd_terminal[0], NULL, STDOUT_FILENO, NULL, 32768, 0);
  assert(ret != -1);
  // 文件管道读出 -> 文件
  ret = splice(pipefd_file[0], NULL, filefd, NULL, 327668, 0);
  assert(ret != -1);

  close(filefd);
  close(pipefd_file[0]);
  close(pipefd_file[1]);
  close(pipefd_terminal[0]);
  close(pipefd_terminal[1]);
  return 0;
}