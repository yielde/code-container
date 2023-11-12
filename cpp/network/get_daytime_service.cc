#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " hostname service_name" << endl;
    return 0;
  }
  char* hostname = argv[1];
  // 获取主机信息
  struct hostent* hostinfo = gethostbyname(hostname);
  assert(hostinfo);

  // 获取daytime service信息
  struct servent* servinfo = getservbyname("daytime", "tcp");
  assert(servinfo);
  cout << "daytime port is " < < ntohs()< < endl;
}