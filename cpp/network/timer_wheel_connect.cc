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

#include "timer_wheel.h"

using namespace std;

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMEINTVAL 1
static time_wheel t_wheel;
static int pipefd[2];
static int epollfd = 0;

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
  int old_errno = errno;
  char msg = sig;
  send(pipefd[1], (char *)&msg, 1, 0);
  errno = old_errno;
}

void addsig(int sig) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_flags |= SA_RESTART;
  sa.sa_handler = sig_handler;
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig, &sa, NULL) != -1);
}

void cb_func(client_data *user) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, user->sockfd, 0);
  assert(user);
  close(user->sockfd);
  cout << "close socket fd " << user->sockfd << endl;
}
void time_handler() {
  t_wheel.tick();
  alarm(TIMEINTVAL);
}
int main(int argc, char *argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);
  char stop_server = false;
  bool timeout = false;

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  inet_pton(AF_INET, ip, &server_addr.sin_addr);
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    cout << "server error" << endl;
    return 1;
  }
  int ret =
      bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 6);
  assert(ret != -1);

  int epollfd = epoll_create(6);
  addfd(epollfd, listenfd);
  epoll_event events[FD_LIMIT];
  addsig(SIGTERM);
  addsig(SIGALRM);
  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
  setnonblocking(pipefd[1]);
  addfd(epollfd, pipefd[0]);
  client_data *users = new client_data[FD_LIMIT];

  alarm(TIMEINTVAL);
  while (!stop_server) {
    int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if ((number < 0) && (errno != EINTR)) {
      cout << "epoll error" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if (sockfd == listenfd) {
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int connfd =
            accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        addfd(epollfd, connfd);
        users[connfd].sockfd = connfd;
        users[connfd].address = cli_addr;
        tw_timer *timer = t_wheel.add_timer(TIMEINTVAL * 5);
        timer->user_data = &users[connfd];
        timer->cb_func = cb_func;
        users[connfd].timer = timer;
      } else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN)) {
        int sig;
        char signals[1024];
        ret = recv(sockfd, signals, sizeof(signals), 0);
        if (ret == -1) {
          continue;
        } else if (ret == 0) {
          continue;
        } else {
          for (int i = 0; i < ret; ++i) {
            sig = signals[i];
            switch (sig) {
              case SIGALRM:
                timeout = true;
                cout << "set timeout" << endl;
                break;

              case SIGTERM:
                stop_server = true;
                cout << "set stop server" << endl;
            }
          }
        }
      } else if (events[i].events & EPOLLIN) {
        memset(users[sockfd].buf, '\0', BUFFER_SIZE);
        ret = recv(sockfd, users[sockfd].buf, BUFFER_SIZE - 1, 0);
        cout << "get " << ret << "bytes of client data " << users[sockfd].buf
             << " from " << sockfd << endl;
        tw_timer *timer = users[sockfd].timer;
        if ((ret < 0) && (errno != EAGAIN)) {
          cb_func(&users[sockfd]);
          if (timer) {
            t_wheel.delete_timer(timer);
          }
        } else if (ret == 0) {
          cb_func(&users[sockfd]);
          if (timer) {
            t_wheel.delete_timer(timer);
          }
        } else {
          if (timer) {
            t_wheel.delete_timer(timer);
            tw_timer *new_timer = t_wheel.add_timer(TIMEINTVAL * 5);
            new_timer->user_data = &users[sockfd];
            new_timer->cb_func = cb_func;
            users[sockfd].timer = new_timer;
          }
        }
      }
    }
    if (timeout) {
      time_handler();
      timeout = false;
    }
  }
  close(listenfd);
  close(epollfd);
  close(pipefd[0]);
  close(pipefd[1]);
  delete[] users;
}