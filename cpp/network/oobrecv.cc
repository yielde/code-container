#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#define BUFESIZE 1024
using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "Usage: " << basename(argv[0]) << " ip_address port_number" << endl;
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  assert(sock > 0);

  const char *ip = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &server_address.sin_addr);
  server_address.sin_port = htons(port);

  int ret =
      bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
  if (ret == -1)
    cout << "errno is: " << errno << "means: " << gai_strerror(errno) << endl;
  ret = listen(sock, 5);
  assert(ret != -1);
  struct sockaddr_in client_address;
  socklen_t client_addrlength = sizeof(client_address);
  int connfd =
      accept(sock, (struct sockaddr *)&client_address, &client_addrlength);
  if (connfd < 0)
    cout << "errno is: " << errno << "means: " << strerror(errno) << endl;

  else {
    char buffer[BUFESIZE];
    memset(buffer, '\0', BUFESIZE);
    ret = recv(connfd, buffer, BUFESIZE - 1, 0);
    cout << "got " << ret << " bytes"
         << "of normal data"
         << "'" << buffer << "'" << endl;

    memset(buffer, '\0', BUFESIZE);
    // 接收带外数据
    ret = recv(connfd, buffer, BUFESIZE - 1, MSG_OOB);
    cout << "got " << ret << " bytes"
         << "of oob data"
         << "'" << buffer << "'" << endl;

    memset(buffer, '\0', BUFESIZE);
    ret = recv(connfd, buffer, BUFESIZE - 1, 0);
    cout << "got " << ret << " bytes"
         << "of normal data"
         << "'" << buffer << "'" << endl;
    close(connfd);
  }
  close(sock);
  return 0;
}