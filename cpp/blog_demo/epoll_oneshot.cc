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
#define BUFFERSIZE 1024

static int epollfd = 0;

void setnonblocking(int fd) {
  int old_fd_option = fcntl(fd, F_GETFL);
  int new_fd_option = old_fd_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_fd_option);
}

void register_epoll(int epollfd, int fd, bool newfd = false,
                    bool oneshot = false) {
  epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;  // 读事件、ET工作模式
  if (oneshot) {
    events.events |= EPOLLONESHOT;  // 使用EPOLLONESHOT
  }
  if (newfd) {
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
  } else {
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &events);
  }
  setnonblocking(fd);
}

void *handle_connect(void *arg) {
  pid_t tid = gettid();
  int connfd = *((int *)arg);
  cout << "use thread " << tid << " to handle connect " << connfd << endl;
  char buffer[BUFFERSIZE];
  memset(buffer, '\0', BUFFERSIZE);
  while (true) {
    int bytes = recv(connfd, buffer, BUFFERSIZE - 1, 0);
    if (bytes == 0) {
      cout << "the other peer close connection" << endl;
      close(connfd);
      break;
    } else if (bytes < 0) {
      if (errno == EAGAIN) {
        cout << connfd << " Temporarily unavailable, read later" << endl;
        register_epoll(epollfd, connfd, false,
                       true);  // 重置该连接fd的EPOLLONESHOT
        break;
      } else {
        cout << "read " << connfd << " failure" << endl;
        close(connfd);
      }
    } else {
      cout << "thread " << tid << " recve " << bytes
           << " bytes from connection " << connfd << ", content: " << buffer
           << endl;
      sleep(10);
    }
  }
  cout << "thread " << tid << " end handle connect " << connfd << endl;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }
  // 创建server端socket
  const char *ip = argv[1];
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, ip, &serv_addr.sin_addr);

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    cout << "create socket error" << endl;
    return 1;
  }
  int ret = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  assert(ret != -1);
  // epoll_event数组，用来接收返回的就绪fd
  epoll_event events[MAX_EVENT_NUMBER];
  // 创建epoll
  epollfd = epoll_create(5);
  if (epollfd < 0) {
    cout << "create epoll error" << endl;
    close(listenfd);
    return 1;
  }
  // listenfd 无需使用EPOLLONESHOT
  register_epoll(epollfd, listenfd, true, false);
  while (true) {
    // 等待事件触发
    int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
    for (int i = 0; i < number; i++) {
      int sockfd = events[i].data.fd;
      if ((sockfd == listenfd) && (events[i].events & EPOLLIN)) {
        // 接收客户端连接
        struct sockaddr cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int connfd =
            accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if (connfd < 0) {
          cout << "accept connect failure" << endl;
          continue;
        }
        // 新的连接使用EPOLLONESHOT属性
        register_epoll(epollfd, connfd, true, true);

        // 新的连接不使用EPOLLONESHOT属性
        // register_epoll(epollfd, connfd, true, false);
      } else if (events[i].events & EPOLLIN) {
        // 已建立的连接有数据请求
        pthread_t thread;
        // 创建线程处理连接数据，传入sockfd参数以便重置EPOLLONESHOT
        pthread_create(&thread, NULL, handle_connect, &sockfd);
      } else {
        cout << "other errors" << endl;
      }
    }
  }
  close(listenfd);
  close(epollfd);

  return 0;
}
