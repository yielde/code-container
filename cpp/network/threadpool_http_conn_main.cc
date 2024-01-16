#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "locker.h"
#include "threadpool_half_sync_proactor.h"
#include "threadpool_http_conn.h"
using namespace std;
#define MAX_FD 655355
#define MAX_EVENT_NUMBER 10000

extern int addfd(int epollfd, int fd, bool one_shot);
extern int removefd(int epollfd, int fd);

void addsig(int sig, void(handler)(int), bool restart = true) {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = handler;
  if (restart) {
    sa.sa_flags |= SA_RESTART;
  }
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig, &sa, NULL) != -1);
}

void show_error(int connfd, const char* info) {
  cout << info << endl;
  send(connfd, info, strlen(info), 0);
  close(connfd);
}

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << "ip port" << endl;
    return 1;
  }
  const char* ip = argv[1];
  int port = atoi(argv[2]);
  addsig(SIGPIPE, SIG_IGN);
  threadpool<http_conn>* pool = NULL;
  try {
    pool = new threadpool<http_conn>;
  } catch (...) {
    return 1;
  }
  http_conn* users = new http_conn[MAX_FD];
  assert(users);
  int user_count = 0;
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  struct linger tmp = {1, 0};
  setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
  int ret = 0;
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &serv_addr.sin_addr);
  serv_addr.sin_port = htons(port);
  ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  assert(ret >= 0);
  ret = listen(listenfd, 5);
  assert(ret >= 0);
  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd, listenfd, false);
  http_conn::m_epollfd = epollfd;

  while (true) {
    int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    if ((number < 0) && (errno != EINTR)) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if (sockfd == listenfd) {
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int connfd =
            accept(listenfd, (struct sockaddr*)&cli_addr, &cli_addr_len);
        if (connfd < 0) {
          cout << "errno is: " << errno << endl;
          continue;
        }
        if (http_conn::m_user_count >= MAX_FD) {
          show_error(connfd, "Internal server busy");
          continue;
        }
        users[connfd].init(connfd, cli_addr);
      } else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
        users[sockfd].close_conn();
      } else if (events[i].events & EPOLLIN) {
        if (users[sockfd].read()) {
          pool->append(users + sockfd);
        } else {
          users[sockfd].close_conn();
        }
      } else if (events[i].events & EPOLLOUT) {
        if (!users[sockfd].write()) {
          users[sockfd].close_conn();
        }
      }
    }
  }
  close(epollfd);
  close(listenfd);
  delete[] users;
  delete pool;
  return 0;
}