/*
  telnet在20秒之内发起连接并断开，
  accept只从队列中拿出连接，不关心连接是ESTABLISHED还是CLOSE_WAIT状态
*/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << basename(argv[0]) << " ip_address port_number" << endl;
    return 1;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  assert(sock >= 0);

  int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sock, 5);
  assert(ret != -1);
  sleep(20);
  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
  if (connfd < 0) cout << "errno is:" << errno << endl;
  // 打印客户端地址信息
  else {
    char remote[INET_ADDRSTRLEN];
    cout << "connected with ip: "
         << inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN)
         << " and port: " << ntohs(client.sin_port) << endl;
    close(connfd);
  }
  close(sock);
  return 0;
}