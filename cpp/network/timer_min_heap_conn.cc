#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

#include "timer_min_heap.h"

#define MAX_FD_LIMIT 65535
#define MAX_USERS 64
#define TIME_SLOT 5

static bool server_stop = false;
static bool timeout = false;
static int pipefd[2];
static time_heap time_h = time_heap(1024);
static int epollfd = 0;

int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_fd_option);
  return old_fd_option;
}

void addfd(int epollfd, int fd) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  setnonblocking(fd);
}

void sig_handler(int sig) {
  cout << "handle sig: " << sig << endl;
  int old_errno = errno;
  char msg = sig;
  send(pipefd[1], (char *)&msg, 1, 0);
  errno = old_errno;
}

void cb_func(client_data *user) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, user->sockfd, 0);
  assert(user);
  close(user->sockfd);
  cout << "close fd " << user->sockfd;
}

void addsig(int sig) {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_flags |= SA_RESTART;
  sa.sa_handler = sig_handler;
  assert(sigaction(sig, &sa, NULL) != -1);
}

void time_handler() {
  time_h.tick();
  alarm(TIME_SLOT);
}

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << "ip port" << endl;
    return 1;
  }
  const char *ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    cout << "socket error !" << endl;
    return 1;
  }
  int ret =
      bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 6);
  assert(ret != -1);

  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);

  assert(ret != -1);
  int epollfd = epoll_create(6);

  addfd(epollfd, listenfd);
  addfd(epollfd, pipefd[0]);
  setnonblocking(pipefd[1]);

  addsig(SIGALRM);
  addsig(SIGTERM);

  epoll_event events[MAX_FD_LIMIT];

  client_data *users = new client_data[MAX_USERS];

  alarm(TIME_SLOT);
  while (!server_stop) {
    ret = epoll_wait(epollfd, events, MAX_FD_LIMIT, -1);
    for (int i = 0; i < ret; ++i) {
      int sockfd = events[i].data.fd;
      if (sockfd == listenfd) {
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int connfd =
            accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if (connfd < 0) {
          cout << "connect error" << endl;
          break;
        }
        heap_timer *timer = new heap_timer(TIME_SLOT);
        users[connfd].address = cli_addr;
        users[connfd].sockfd = connfd;
        users[connfd].timer = timer;
        timer->cb_func = cb_func;
        timer->user_data = &users[connfd];
        time_h.add_timer(timer);
        addfd(epollfd, connfd);
      } else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
        char signals[1024];
        ret = recv(sockfd, signals, sizeof(signals) - 1, 0);
        if ((ret < 0) && (errno != EAGAIN)) {
          continue;
        } else if (ret == 0) {
          continue;
        } else {
          for (int i = 0; i < ret; ++i) {
            int sig = signals[i];
            switch (sig) {
              case SIGALRM:
                cout << "set timeout" << endl;
                timeout = true;
                break;
              case SIGTERM:
                cout << "stop server" << endl;
                server_stop = true;
            }
          }
        }
      } else if (events[i].events & EPOLLIN) {
        memset(users[sockfd].buf, '\0', BUFFER_SIZE);
        ret = recv(sockfd, users[sockfd].buf, BUFFER_SIZE - 1, 0);
        cout << "get " << ret << " bytes "
             << " from " << sockfd << " : " << users[sockfd].buf << endl;
        heap_timer *timer = users[sockfd].timer;
        if ((ret < 0) && (errno != EAGAIN)) {
          cb_func(&users[sockfd]);
          if (timer) {
            time_h.delete_timer(timer);
          }
        } else if (ret == 0) {
          cb_func(&users[sockfd]);
          if (timer) {
            time_h.delete_timer(timer);
          }
        } else {
          time_h.delete_timer(timer);
          heap_timer *new_timer = new heap_timer(TIME_SLOT);
          new_timer->cb_func = cb_func;
          new_timer->user_data = &users[sockfd];
          time_h.add_timer(new_timer);
          users[sockfd].timer = new_timer;
        }
      }
    }
    if (timeout) {
      time_handler();
      timeout = false;
    }
  }
  close(epollfd);
  close(listenfd);
  close(pipefd[0]);
  close(pipefd[1]);
  delete[] users;
  return 0;
}