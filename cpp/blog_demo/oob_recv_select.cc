#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#define BUFFERSIZE 1024
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  // 设置TCP socket server
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  const char *ip = argv[1];
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    cout << "error in create socket" << endl;
    return 1;
  }
  int ret =
      bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 6);
  assert(ret != -1);

  // 接收客户端连接
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int connfd =
      accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (connfd < 0) {
    close(listenfd);
    cout << "accept connect error" << endl;
    return 1;
  }
  // 初始化要用到的select fd集
  fd_set readfds;
  fd_set exceptionfds;
  FD_ZERO(&readfds);
  FD_ZERO(&exceptionfds);
  char buffer[BUFFERSIZE];
  while (true) {
    // 如果是普通数据则触发readfds， 如果是oob数据触发exceptionfds
    FD_SET(connfd, &readfds);
    FD_SET(connfd, &exceptionfds);
    // 注册select， 不关心写fds设置为NULL，timeout NULL为阻塞
    ret = select(connfd + 1, &readfds, NULL, &exceptionfds, NULL);
    if (ret < 0) {
      cout << "select error" << endl;
      break;
    }
    memset(buffer, '\0', BUFFERSIZE);
    if (FD_ISSET(connfd, &readfds)) {
      // 接收普通数据
      int number = recv(connfd, buffer, BUFFERSIZE - 1, 0);
      if (number < 0) {
        cout << "recv normal data error" << endl;
        break;
      } else if (number == 0) {
        cout << "connection closed" << endl;
        break;
      }
      cout << "recv normal data " << number << " bytes: " << buffer << endl;
    }
    memset(buffer, '\0', BUFFERSIZE);
    if (FD_ISSET(connfd, &exceptionfds)) {
      // 接收带外数据
      int number = recv(connfd, buffer, BUFFERSIZE - 1, MSG_OOB);
      if (number < 0) {
        cout << "recv oob data error" << endl;
        break;
      } else if (number == 0) {
        cout << "connection closed" << endl;
        break;
      }
      cout << "recv oob data " << number << " bytes: " << buffer << endl;
    }
  }
  close(listenfd);
  close(connfd);
  return 0;
}