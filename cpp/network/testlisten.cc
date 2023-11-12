#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;
static bool stop = false;

static void handle_item(int sig) { stop = true; }

// 设定backlog为3，可以建立4条连接，或ESTABLISHED和CLOSE_WAIT一共四条，后续的telnet发起连接会timeout
int main(int argc, char *argv[]) {
  signal(SIGTERM, handle_item);
  if (argc < 3) {
    cout << "usage: " << basename(argv[0]) << " ip_address port_number backlog"
         << endl;
    return 1;
  }
  const char *ip = argv[1];
  int port = atoi(argv[2]);
  int backlog = atoi(argv[3]);
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  assert(sock > 0);

  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  // ip地址文本格式转换为网络字节序的2进制地址
  inet_pton(AF_INET, ip, &address.sin_addr);
  // 主机字节序转为网络字节序
  address.sin_port = htons(port);
  // 专有地址例如sockaddr_in在使用时都需要转为sockaddr类型
  int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sock, backlog);
  assert(ret != -1);
  while (!stop) {
    sleep(1);
  }
  close(sock);
  return 0;
}