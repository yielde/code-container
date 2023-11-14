#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage:" << argv[0] << " ip_address port_number" << endl;
    return 0;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in address, client_addr;
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sockfd, 2);
  assert(ret != -1);
  socklen_t client_addr_length = sizeof(client_addr);
  int conn =
      accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_length);
  if (conn < 0)
    cout << "connect error: " << errno << endl;
  else {
    string hello = "hello client";
    send(conn, hello.data(), sizeof(hello), 0);
    close(conn);
  }
  close(sockfd);
  return 0;
}