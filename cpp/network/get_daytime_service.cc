#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " hostname" << endl;
    return 0;
  }
  char* hostname = argv[1];
  // 获取主机信息
  struct hostent* hostinfo = gethostbyname(hostname);
  assert(hostinfo);

  /*
    获取daytime service信息，自定义一个服务，
    编辑/etc/services, my	        8889/tcp
  */
  struct servent* servinfo = getservbyname("my", "tcp");
  assert(servinfo);
  cout << "daytime port is " << ntohs(servinfo->s_port) << endl;
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = servinfo->s_port;

  address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int result = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
  assert(result != -1);
  char buffer[128];
  result = recv(sockfd, buffer, sizeof(buffer), 0);
  cout << "resceived: " << result << endl;
  assert(result > 0);
  buffer[result] = '\0';
  cout << "the day time is " << buffer << endl;
  close(sockfd);
  return 0;
}