#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#define BUFFERSIZE 1024
using namespace std;
// 存放pollfd结构数组
pollfd fds[2];

void setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = O_NONBLOCK | old_fd_option;
  fcntl(fd, F_SETFL, new_fd_option);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "usage: " << argv[0] << "filename1 filename2" << endl;
    return 1;
  }
  // 打开创建好的文件
  int fd1 = open(argv[1], O_RDONLY);
  int fd2 = open(argv[2], O_RDONLY);
  // 设置pollfd结构
  fds[0].fd = fd1;
  fds[0].events = POLLIN | POLLERR;
  fds[0].revents = 0;

  fds[1].fd = fd2;
  fds[1].events = POLLIN | POLLERR;
  fds[1].revents = 0;
  // 设置fd为非阻塞，方便看读取的效果，否则会阻塞在read调用上
  setnonblocking(fd1);
  setnonblocking(fd2);

  char buffer[BUFFERSIZE];
  int number = 0;
  while (true) {
    // 创建poll
    int ret = poll(fds, 2, -1);
    if (ret < 0) {
      cout << "poll error" << endl;
      break;
    }
    for (int i = 0; i < 2; ++i) {
      pollfd fd = fds[i];
      if (fd.revents & POLLERR) {
        cout << "poll error fd: " << fd.fd << endl;
        continue;
        // 如果fd可读
      } else if (fd.revents & POLLIN) {
        // 每次poll事件清空缓冲区
        bzero(buffer, BUFFERSIZE);
        while ((number = read(fd.fd, buffer, BUFFERSIZE)) > 0) {
          cout << "read " << number << " bytes from file " << argv[i + 1]
               << " content: " << buffer << endl;
        }
      }
    }
  }
  close(fd1);
  return 0;
}