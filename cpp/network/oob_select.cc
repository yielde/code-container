#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  char *ip = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in server_address, client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_address.sin_addr);
  int listenfd, connfd;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  int ret = bind(listenfd, (struct sockaddr *)&server_address,
                 sizeof(server_address));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  assert(ret != -1);
  socklen_t client_addr_len = sizeof(client_address);
  connfd =
      accept(listenfd, (struct sockaddr *)&client_address, &client_addr_len);
  if (connfd < 0) {
    cout << "errno is : " << errno << endl;
  } else {
    char buff[1024];
    fd_set read_fds;
    fd_set exception_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);

    while (1) {
      memset(buff, '\0', sizeof(buff));
      // fd_set会被内核修改，每次调用之前需要重新设置
      FD_SET(connfd, &read_fds);
      FD_SET(connfd, &exception_fds);
      ret = select(connfd + 1, &read_fds, NULL, &exception_fds, NULL);
      if (ret < 0) {
        cout << "selection failure" << endl;
        break;
      }
      // read_fds
      if (FD_ISSET(connfd, &read_fds)) {
        ret = recv(connfd, buff, sizeof(buff) - 1, 0);
        if (ret <= 0) {
          break;
        }
        cout << "get " << ret << " bytes of normal data: " << buff << endl;
      }
      memset(buff, '\0', sizeof(buff));
      if (FD_ISSET(connfd, &exception_fds)) {
        ret = recv(connfd, buff, sizeof(buff) - 1, MSG_OOB);  // 处理带外数据
        if (ret <= 0) {
          cout << "nothing" << endl;
          break;
        }
        cout << "get " << ret << " bytes of oob data: " << buff << endl;
      }
    }
    close(connfd);
  }
  close(listenfd);
  return 0;
}