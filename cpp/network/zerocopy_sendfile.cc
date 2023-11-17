#include <arpa/inet.h>
#include <fcntl.h>
#include <libgen.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
  if (argc <= 3) {
    cout << "usage: " << basename(argv[0])
         << "ip_address port_number filename\n"
         << endl;
    return 0;
  }
  const char* ip = argv[1];
  int port = atoi(argv[2]);
  const char* filename = argv[3];
  int filefd = open(filename, O_RDONLY);
  assert(filefd > 0);
  struct stat stat_buf;
  fstat(filefd, &stat_buf);

  struct sockaddr_in address, client_address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);
  int ret = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sockfd, 3);
  socklen_t client_address_length = sizeof(client_address);
  int connfd =
      accept(sockfd, (struct sockaddr*)&client_address, &client_address_length);
  if (connfd < 0) {
    cout << "error is: " << errno << strerror(errno) << endl;
  } else {
    // 内核发送，offset为NULL表示从文件最开始发送
    sendfile(connfd, filefd, NULL, stat_buf.st_size);
    close(connfd);
  }
  close(sockfd);
  close(filefd);
  return 0;
}