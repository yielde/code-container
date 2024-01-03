#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <timer_signal_liner.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 5

static int pipefd[2];
static sort_time_lst timer_lst;
static int epollfd = 0;

int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = O_NONBLOCK | old_fd_option;
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

void timer_handler() {
  timer_lst.tick();
  alarm(TIMESLOT);
}

void cb_func(client_data *user_data) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
  assert(user_data);
  close(user_data->sockfd);
  cout << "close fd " << user_data->sockfd << endl;
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
  ret = listen(listenfd, 6);
  assert(ret != -1);
  epoll_event events[MAX_EVENT_NUMBER];
  int epollfd = epoll_create(5);
  assert(epollfd != -1);
  addfd(epollfd, listenfd);
  ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
  assert(ret != -1);
  setnonblocking(pipefd[1]);
  addfd(epollfd, pipefd[0]);

  addsig(SIGALRM);
  addsig(SIGTERM);
  bool stop_server = false;
  client_data *users = new client_data[FD_LIMIT];
  bool timeout = false;
  alarm(TIMESLOT);

  while (!stop_server) {
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
            accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        addfd(epollfd, connfd);
        users[connfd].address = cli_addr;
        users[connfd].sockfd = connfd;
        util_timer *timer = new util_timer;
        timer->user_data = &users[connfd];
        timer->cb_func = cb_func;
        time_t cur = time(NULL);
        timer->expire = cur + 3 * TIMESLOT;
        users[connfd].timer = timer;
        timer_lst.add_timer(timer);
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
              case SIGALRM:
                timeout = true;
                cout << "setting timeout" << endl;
                break;
              case SIGTERM:
                stop_server = true;
                cout << "stopping server" << endl;
            }
          }
        }
      } else if (events[i].events & EPOLLIN) {
        memset(users[sockfd].buf, '\0', BUFFER_SIZE);
        ret = recv(sockfd, users[sockfd].buf, BUFFER_SIZE - 1, 0);
        cout << "get " << ret << "bytes of client data " << users[sockfd].buf
             << " from " << sockfd << endl;
        util_timer *timer = users[sockfd].timer;
        if (ret < 0) {
          if (errno != EAGAIN) {
            cb_func(&users[sockfd]);
            if (timer) {
              timer_lst.delete_timer(timer);
            }
          }
        } else if (ret == 0) {
          cb_func(&users[sockfd]);
          if (timer) {
            timer_lst.delete_timer(timer);
          }
        } else {
          if (timer) {
            time_t cur = time(NULL);
            timer->expire = cur + 3 * TIMESLOT;
            cout << "adjust timer once" << endl;
            timer_lst.adjust_timer(timer);
          }
        }
      }
    }
    if (timeout) {
      timer_handler();
      timeout = false;
    }
  }
  close(listenfd);
  close(pipefd[0]);
  close(pipefd[1]);
  delete[] users;
  return 0;
}