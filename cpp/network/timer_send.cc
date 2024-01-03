#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>

using namespace std;

int timeout_connect(const char* ip, int port, int time) {
  int ret = 0;
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct timeval timeout;
  timeout.tv_sec = time;
  timeout.tv_usec = 0;

  ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout,
                   (socklen_t)sizeof(timeout));
  assert(ret != -1);
  ret = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (ret == -1) {
    if (errno == EINPROGRESS) {
      cout << "connect timeout !!!" << endl;
      return -1;
    }
    cout << "error when connecting to server" << endl;
    return -1;
  }
  return sockfd;
}

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  const char* ip = argv[1];
  int port = atoi(argv[2]);
  int sockfd = timeout_connect(ip, port, 10);
  if (sockfd < 0) {
    return 1;
  }
  return 0;
}