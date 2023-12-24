#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
using namespace std;
#define BUFFER_SIZE 64
int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    cout << "connection failed" << endl;
    close(sockfd);
    return 1;
  }

  pollfd fds[2];
  fds[0].fd = 0;
  fds[0].events = POLLIN;
  fds[0].revents = 0;

  fds[1].fd = sockfd;
  fds[1].events = POLLIN | POLLRDHUP;
  fds[1].revents = 0;

  char read_buf[BUFFER_SIZE];
  int pipefd[2];
  int ret = pipe(pipefd);
  assert(ret != -1);
  while (1) {
    ret = poll(fds, 2, -1);
    if (ret < 0) {
      cout << "poll failure" << endl;
      break;
    }
    if (fds[1].revents & POLLRDHUP) {
      cout << "server close the connection" << endl;
      break;
    } else if (fds[1].revents & POLLIN) {
      memset(read_buf, '\0', sizeof(read_buf));
      recv(fds[1].fd, read_buf, BUFFER_SIZE - 1, 0);
      cout << read_buf << endl;
    }
    if (fds[0].revents & POLLIN) {
      ret = splice(
          0, NULL, pipefd[1], NULL, 32768,
          SPLICE_F_MORE |
              SPLICE_F_MOVE);  // 集中多个splice后传输数据 | 移动而不是复制数据
      ret = splice(pipefd[0], NULL, sockfd, NULL, 32768,
                   SPLICE_F_MORE | SPLICE_F_MOVE);
    }
  }
  close(sockfd);
  return 0;
}