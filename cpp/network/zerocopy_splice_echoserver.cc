#include <arpa/inet.h>
#include <fcntl.h>
#include <libgen.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    cout << "usage: " << basename(argv[0]) << " ip_address port_number" << endl;
    return 1;
  }
  const char* ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in address, client;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);

  int ret = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sockfd, 3);
  assert(ret != -1);

  socklen_t client_address_length = sizeof(client);
  int connfd =
      accept(sockfd, (struct sockaddr*)&client, &client_address_length);
  if (connfd < 0) {
    cout << "error :" << errno << " " << strerror(errno) << endl;
  } else {
    int pipefd[2];
    ret = pipe(pipefd);
    assert(ret != -1);
    ret = splice(connfd, NULL, pipefd[1], NULL, 32768, 0);
    assert(ret != -1);
    ret = splice(pipefd[0], NULL, connfd, NULL, 32768, 0);
    assert(ret != -1);
    close(connfd);
  }
  close(sockfd);
  return 0;
}