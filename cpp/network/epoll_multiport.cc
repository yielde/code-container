#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

int setnonblocking(int fd) {
  int old_option = fcntl(fd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

void addfd(int epollfd, int fd) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  setnonblocking(fd);
}

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  const char *ip = argv[1];
  int port = atoi(argv[2]);
  int ret = 0;

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  ret = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  assert(ret != -1);

  int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  assert(udpfd >= 0);
  ret = bind(udpfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd, listenfd);
  addfd(epollfd, udpfd);
  while (1) {
    int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if (number < 0) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if (listenfd == sockfd) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connfd =
            accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        addfd(epollfd, connfd);
      } else if (sockfd == udpfd) {
        char buf[UDP_BUFFER_SIZE];
        memset(buf, '\0', UDP_BUFFER_SIZE);
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE - 1, 0,
                       (struct sockaddr *)&client_addr,
                       &client_addr_len);  // 无连接获取对端地址使用recvfrom
        if (ret > 0) {
          sendto(udpfd, buf, UDP_BUFFER_SIZE - 1, 0,
                 (struct sockaddr *)&client_addr, client_addr_len);
        }
      } else if (events[i].events & EPOLLIN) {
        char buf[TCP_BUFFER_SIZE];
        while (1) {
          memset(buf, '\0', TCP_BUFFER_SIZE);
          ret = recv(sockfd, buf, TCP_BUFFER_SIZE - 1, 0);
          if (ret < 0) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
              break;
            }
            close(sockfd);
            break;
          } else if (ret == 0) {
            close(sockfd);
          } else {
            send(sockfd, buf, ret, 0);
          }
        }
      } else {
        cout << "error !!!" << endl;
      }
    }
  }
  close(listenfd);
  return 0;
}