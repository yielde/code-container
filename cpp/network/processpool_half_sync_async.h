#ifndef PROCESSPOOL_HALF_SYNC_ASYNC
#define PROCESSPOOL_HALF_SYNC_ASYNC

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>
using namespace std;
class process {
 public:
  process() : m_pid(-1) {}
  int m_pipefd[2];
  pid_t m_pid;
};

template <typename T>
class processpool {
 private:
  processpool(int listenfd, int process_number = 8);

 public:
  static processpool<T> *create(int listenfd, int process_number = 8) {
    if (!m_instance) {
      m_instance = new processpool<T>(listenfd, process_number);
    }
    return m_instance;
  }
  ~processpool() { delete[] m_sub_process; }
  void run();

 private:
  static const int MAX_PROCESS_NUMBER = 16;
  static const int USER_PER_PROCESS = 655350;
  static const int MAX_EVENT_NUMBER = 10000;
  int m_process_number;
  int m_idx;
  int m_epollfd;
  int m_listenfd;
  int m_stop;
  static processpool<T> *m_instance;
  process *m_sub_process;

  void setup_sig_pipe();
  void run_parent();
  void run_child();
};

template <typename T>
processpool<T> *processpool<T>::m_instance = NULL;

static int sig_pipefd[2];

static int setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_fd_option);
  return old_fd_option;
}

static void addfd(int epollfd, int fd) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  setnonblocking(fd);
}

static void removefd(int epollfd, int fd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
  close(fd);
}

static void sig_handler(int sig) {
  int save_errno = errno;
  int msg = sig;
  send(sig_pipefd[1], (char *)&msg, 1, 0);
  errno = save_errno;
}

static void addsig(int sig, void(handler)(int), bool restart = true) {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = sig_handler;
  if (restart) {
    sa.sa_flags |= SA_RESTART;
  }
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig, &sa, NULL) != -1);
}

template <typename T>
processpool<T>::processpool(int listenfd, int process_number)
    : m_listenfd(listenfd),
      m_process_number(process_number),
      m_idx(-1),
      m_stop(false) {
  assert((process_number > 0) && (process_number <= MAX_PROCESS_NUMBER));
  m_sub_process = new process[process_number];
  assert(m_sub_process);
  for (int i = 0; i < process_number; ++i) {
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd);
    assert(ret == 0);
    m_sub_process[i].m_pid = fork();
    assert(m_sub_process[i].m_pid >= 0);
    if (m_sub_process[i].m_pid > 0) {
      close(m_sub_process[i].m_pipefd[1]);
      continue;
    } else {
      close(m_sub_process[i].m_pipefd[0]);
      m_idx = i;
      break;
    }
  }
}

template <typename T>
void processpool<T>::setup_sig_pipe() {
  m_epollfd = epoll_create(5);
  assert(m_epollfd != -1);
  int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sig_pipefd);
  assert(ret != -1);
  setnonblocking(sig_pipefd[1]);
  addfd(m_epollfd, sig_pipefd[0]);
  addsig(SIGCHLD, sig_handler);
  addsig(SIGTERM, sig_handler);
  addsig(SIGINT, sig_handler);
  addsig(SIGPIPE, sig_handler);
}

template <typename T>
void processpool<T>::run() {
  if (m_idx != -1) {
    run_child();
    return;
  }
  run_parent();
}

template <typename T>
void processpool<T>::run_child() {
  setup_sig_pipe();
  int pipefd = m_sub_process[m_idx].m_pipefd[1];
  addfd(m_epollfd, pipefd);
  epoll_event events[MAX_EVENT_NUMBER];
  T *users = new T[USER_PER_PROCESS];
  assert(users);
  int number = 0;
  int ret = -1;
  while (!m_stop) {
    number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
    if ((number < 0) && (errno != EINTR)) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if ((sockfd == pipefd) && (events[i].events & EPOLLIN)) {
        int client = 0;
        ret = recv(sockfd, (char *)&client, sizeof(client), 0);
        if (((ret < 0) && (errno != EAGAIN)) || ret == 0) {
          continue;
        } else {
          struct sockaddr_in cli_addr;
          socklen_t cli_addr_len = sizeof(cli_addr);
          int connfd =
              accept(m_listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
          if (connfd < 0) {
            cout << "errno is " << errno << endl;
            continue;
          }
          addfd(m_epollfd, connfd);
          users[connfd].init(m_epollfd, connfd, cli_addr);
        }
      } else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN)) {
        int sig;
        char signals[1024];
        ret = recv(sockfd, signals, sizeof(signals), 0);
        if (ret <= 0) {
          continue;
        } else {
          for (int i = 0; i < ret; ++i) {
            switch (signals[i]) {
              case SIGCHLD: {
                pid_t pid;
                int stat;
                while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                  continue;
                }
                break;
              }
              case SIGTERM:
              case SIGINT: {
                m_stop = true;
                break;
              }
              default: {
                break;
              }
            }
          }
        }
      } else if (events[i].events & EPOLLIN) {
        users[sockfd].process();
      } else {
        continue;
      }
    }
  }
  delete[] users;
  users = NULL;
  close(pipefd);
  close(m_epollfd);
}

template <typename T>
void processpool<T>::run_parent() {
  setup_sig_pipe();
  addfd(m_epollfd, m_listenfd);
  epoll_event events[MAX_EVENT_NUMBER];
  int sub_process_counter = 0;
  int new_conn = 1;
  int number = 0;
  int ret = -1;
  while (!m_stop) {
    number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
    if ((number < 0) && (errno != EINTR)) {
      cout << "epoll failure" << endl;
      break;
    }
    for (int i = 0; i < number; ++i) {
      int sockfd = events[i].data.fd;
      if (sockfd == m_listenfd) {
        int i = sub_process_counter;
        do {
          if (m_sub_process[i].m_pid != -1) {
            break;
          }
          i = (i + 1) % m_process_number;
        } while (i != sub_process_counter);
        if (m_sub_process[i].m_pid == -1) {
          m_stop = true;
          break;
        }
        sub_process_counter = (i + 1) % m_process_number;
        send(m_sub_process[i].m_pipefd[0], (char *)&new_conn, sizeof(new_conn),
             0);
        cout << "send reuquest to child " << i << endl;
      } else if ((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN)) {
        int sig;
        char signals[1024];
        ret = recv(sockfd, signals, sizeof(signals), 0);
        if (ret <= 0) {
          continue;
        } else {
          for (int i = 0; i < ret; ++i) {
            switch (signals[i]) {
              case SIGCHLD: {
                pid_t pid;
                int stat;
                while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                  for (int i = 0; i < m_process_number; ++i) {
                    if (m_sub_process[i].m_pid == pid) {
                      cout << "child " << i << " join" << endl;
                      close(m_sub_process[i].m_pipefd[0]);
                      m_sub_process[i].m_pid = -1;
                    }
                  }
                }
                m_stop = true;
                for (int i = 0; i < m_process_number; ++i) {
                  if (m_sub_process[i].m_pid != -1) {
                    m_stop = false;
                  }
                }
                break;
              }
              case SIGTERM:
              case SIGINT: {
                cout << "kill all the child now" << endl;
                for (int i = 0; i < m_process_number; ++i) {
                  int pid = m_sub_process[i].m_pid;
                  if (pid != -1) {
                    kill(pid, SIGTERM);
                  }
                }
                break;
              }
              default: {
                break;
              }
            }
          }
        }
      } else {
        continue;
      }
    }
  }
  close(m_epollfd);
}
#endif