#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << basename(argv[0]) << "ip_address port_number" << endl;
    return 1;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);
  int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
  if (ret == -1) {
    cout << "error: " << errno << " " << strerror(errno) << endl;
    return 1;
  }
  ret = listen(sockfd, 3);
  assert(ret != -1);

  struct sockaddr_in client;
  bzero(&client, sizeof(client));
  socklen_t client_address_length = sizeof(client);
  int connfd =
      accept(sockfd, (struct sockaddr *)&client, &client_address_length);
  if (connfd < 0) {
    cout << "errno is: " << errno << " " << strerror(errno) << endl;
  } else {
    close(STDOUT_FILENO);
    dup(connfd);              // fd 为 1
    cout << "hello" << endl;  // 将stdout重定向到了connfd，所以发给客户端hello
    close(connfd);
  }
  close(sockfd);
  return 0;
}