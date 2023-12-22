#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

using namespace std;
// 使用ET模式应当设置fd为非阻塞状态，避免读写操作没有后续事件而持续阻塞
int setnonblocking(int fd) {
  int old_option = fcntl(fd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

// 注册EPOLLIN到epollfd
void addfd(int epollfd, int fd, bool enable_et) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN;
  if (enable_et) {
    event.events |= EPOLLET;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}
// 水平触发epoll
void lt(epoll_event *events, int number, int epollfd, int listenfd) {
  char buf[BUFFER_SIZE];
  for (int i = 0; i < number; i++) {
    int sockfd = events[i].data.fd;
    if (sockfd == listenfd) {
      struct sockaddr_in client_addr;
      socklen_t client_addr_len = sizeof(client_addr);
      int connfd =
          accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
      addfd(epollfd, connfd, false);
    } else if (events[i].events & EPOLLIN) {
      cout << "event trigger once\n" << endl;
      memset(buf, '\0', BUFFER_SIZE);
      int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
      if (ret <= 0) {
        close(sockfd);
        continue;
      }
      cout << "get " << ret << " bytes of content: " << buf << endl;
    } else {
      cout << "something else happened" << endl;
    }
  }
}

void et(epoll_event *events, int number, int epollfd, int listenfd) {
  char buf[BUFFER_SIZE];
  for (int i = 0; i < number; i++) {
    int sockfd = events[i].data.fd;
    if (sockfd == listenfd) {
      struct sockaddr_in cli_addr;
      socklen_t cli_addr_len = sizeof(cli_addr);
      int connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
      addfd(epollfd, connfd, true);
    } else if (events[i].events & EPOLLIN) {
      cout << "event trigger onece" << endl;
      while (1) {
        memset(buf, '\0', BUFFER_SIZE);
        int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
        if (ret < 0) {
          if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            cout << "read later" << endl;
            break;
          }
          close(sockfd);
          break;
        } else if (ret == 0) {
          close(sockfd);
        } else {
          cout << "get " << ret << " bytes of content: " << buf << endl;
        }
      }
    } else {
      cout << "error !!!" << endl;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  const char *ip = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  int ret =
      bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  assert(ret != -1);

  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd, listenfd, true);
  while (1) {
    int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    cout << "epoll ret : " << ret << endl;
    if (ret < 0) {
      cout << "epoll failure" << endl;
      break;
    }
    // lt(events, ret, epollfd, listenfd);
    et(events, ret, epollfd, listenfd);
  }

  close(listenfd);
}