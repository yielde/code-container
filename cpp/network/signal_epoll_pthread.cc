#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#define MAX_EVENT_NUMBER 1024

using namespace std;

static int pipefd[2];

int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_fd_option);
  return old_fd_option;
}

void addfd(int epollfd, int fd) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLET;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}

void sig_handler(int sig) {
  int save_errno = errno;
  int msg = sig;
  send(pipefd[1], (char *)&msg, 1, 0);
  errno = save_errno;
}

void addsig(int sig) {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = sig_handler;
  sa.sa_flags |= SA_RESTART;
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig, &sa, NULL) != -1);
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
  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  ret = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
  setnonblocking(pipefd[1]);
  addfd(epollfd, pipefd[0]);
  addfd(epollfd, listenfd);

  addsig(SIGHUP);
  addsig(SIGCHLD);
  addsig(SIGTERM);
  addsig(SIGINT);
  bool stop_server = false;

  while (!stop_server) {
    int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if ((number < 0) && (errno != EINTR)) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if (sockfd == listenfd) {
        struct sockaddr_in cli_address;
        socklen_t cli_address_len = sizeof(cli_address);
        int connfd =
            accept(listenfd, (struct sockaddr *)&cli_address, &cli_address_len);
        addfd(epollfd, connfd);
      } else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
        int sig;
        char signals[1024];
        ret = recv(pipefd[0], signals, sizeof(signals), 0);
        if (ret == -1) {
          continue;
        } else if (ret == 0) {
          continue;
        } else {
          for (int i = 0; i < ret; ++i) {
            switch (signals[i]) {
              case SIGCHLD:
              case SIGHUP: {
                cout << "continue" << endl;
                continue;
              }
              case SIGTERM:
              case SIGINT: {
                stop_server = true;
              }
            }
          }
        }
      } else {
      }
    }
  }

  cout << "close fds" << endl;
  close(listenfd);
  close(pipefd[0]);
  close(pipefd[1]);
  return 0;
}