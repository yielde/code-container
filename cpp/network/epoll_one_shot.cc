#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
using namespace std;

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

struct fds {
  int epollfd;
  int sockfd;
};

int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_fd_option;
}

void addfd(int epollfd, int fd, bool oneshot) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;
  if (oneshot) {
    events.events |= EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  setnonblocking(fd);
}

void reset_onshot(int epollfd, int fd) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &events);
}

void *worker(void *arg) {
  int sockfd = ((fds *)arg)->sockfd;
  int epollfd = ((fds *)arg)->epollfd;
  cout << "start new thread to receive data on fd: " << sockfd << endl;
  char buf[BUFFER_SIZE];
  memset(buf, '\0', sizeof(buf));
  while (1) {
    int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
    if (ret == 0) {
      close(sockfd);
      cout << "closed the connection" << endl;
      break;
    } else if (ret < 0) {
      if (errno == EAGAIN) {
        reset_onshot(epollfd, sockfd);
        cout << "read later" << endl;
        break;
      }
    } else {
      cout << "get content: " << buf << endl;
      sleep(15);
    }
  }
  cout << "end thread receiving data on fd: " << sockfd << endl;
}

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip address" << endl;
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
  addfd(epollfd, listenfd, false);
  while (1) {
    int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if (ret < 0) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < ret; i++) {
      int sockfd = events[i].data.fd;
      if (sockfd == listenfd) {
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int connfd =
            accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        addfd(epollfd, connfd, true);
      } else if (events[i].events & EPOLLIN) {
        pthread_t thread;
        fds fds_for_new_worker;
        fds_for_new_worker.epollfd = epollfd;
        fds_for_new_worker.sockfd = sockfd;
        pthread_create(&thread, NULL, worker, (void *)&fds_for_new_worker);
      } else {
        cout << "error !!!" << endl;
      }
    }
  }
  close(listenfd);
  return 0;
}