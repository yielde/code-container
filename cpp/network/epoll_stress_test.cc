#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;
static const char* request =
    "GET  http://localhost/main.html  HTTP/1.1\r\nConnection:  "
    "keep-alive\r\n\r\nxxxxxxxxxx";

int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_fd_option);
  return old_fd_option;
}

void addfd(int epollfd, int fd) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLOUT | EPOLLET | EPOLLERR;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  setnonblocking(fd);
}

bool write_nbytes(int sockfd, const char* buffer, int len) {
  int bytes_write = 0;
  cout << "write out " << len << " bytes to " << sockfd << endl;
  while (true) {
    bytes_write = send(sockfd, buffer, len, 0);
    if ((bytes_write == -1) || (bytes_write == 0)) {
      return false;
    }
    len -= bytes_write;
    buffer += bytes_write;
    if (len <= 0) {
      return true;
    }
  }
}

bool read_once(int sockfd, char* buffer, int len) {
  int bytes_read = 0;
  bytes_read = recv(sockfd, buffer, len, 0);
  if ((bytes_read == -1) || (bytes_read == 0)) {
    return false;
  }
  cout << "read " << bytes_read << " from socket " << sockfd
       << " which content: " << buffer << endl;
  return true;
}

void start_conn(int epoll_fd, int num, const char* ip, int port) {
  int ret = 0;
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  for (int i = 0; i < num; ++i) {
    sleep(1);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "create socket " << i << endl;
    if (sockfd < 0) {
      cout << "create socket " << i << " failure" << endl;
      continue;
    }
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==
        0) {
      cout << "build connection " << i << endl;
      addfd(epoll_fd, sockfd);
    }
  }
}

void close_conn(int epoll_fd, int sockfd) {
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sockfd, 0);
  close(sockfd);
}

int main(int argc, char* argv[]) {
  assert(argc == 4);
  int epoll_fd = epoll_create(100);
  start_conn(epoll_fd, atoi(argv[3]), argv[1], atoi(argv[2]));
  epoll_event events[10000];
  char buffer[2048];
  while (true) {
    int fds = epoll_wait(epoll_fd, events, 10000, 2000);
    for (int i = 0; i < fds; ++i) {
      int sockfd = events[i].data.fd;
      if (events[i].events & EPOLLIN) {
        if (!read_once(sockfd, buffer, 2048)) {
          close_conn(epoll_fd, sockfd);
        }
        struct epoll_event events;
        events.events = EPOLLOUT | EPOLLET | EPOLLERR;
        events.data.fd = sockfd;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sockfd, &events);
      } else if (events[i].events & EPOLLOUT) {
        if (!write_nbytes(sockfd, request, strlen(request))) {
          close_conn(epoll_fd, sockfd);
        }
        struct epoll_event events;
        events.events = EPOLLIN | EPOLLET | EPOLLERR;
        events.data.fd = sockfd;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sockfd, &events);
      } else if (events[i].events & EPOLLERR) {
        close_conn(epoll_fd, sockfd);
      }
    }
  }
}