
#define _DEFAULT_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

// static void notify_function(union sigval sv);

// 线程处理
// static void setup_notify(mqd_t *mqp) {
//   struct sigevent sig_ev;              // 定义sigevent
//   sig_ev.sigev_notify = SIGEV_THREAD;  // 通知到达，启用线程处理
//   sig_ev.sigev_notify_function = notify_function;  // 处理函数
//   sig_ev.sigev_notify_attributes = NULL;           // 线程属性设置为NULL
//   sig_ev.sigev_value.sival_ptr = mqp;
//   mq_notify(*mqp, &sig_ev);
// }

// static void notify_function(union sigval sv) {
//   mqd_t *mqp = (mqd_t *)sv.sival_ptr;
//   struct mq_attr attr;
//   mq_getattr(*mqp, &attr);
//   int num = 0;
//   char *buf = (char *)malloc(attr.mq_msgsize);  // 保证buf足够存放消息
//   setup_notify(mqp);
//   while ((num = mq_receive(*mqp, buf, attr.mq_msgsize, NULL)) >= 0) {
//     fprintf(stderr, "receive %d bytes, content: %s", num, buf);
//   }
// }

// epoll模式
void add_epoll(int epollfd, int fd) {
  struct epoll_event events;
  events.data.fd = fd;
  events.events = EPOLLIN | EPOLLET;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &events);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s /mqname", argv[0]);
    return 1;
  }
  mqd_t mq;             // 声明mq标识符
  struct mq_attr attr;  // 声明消息属性
  if ((mq = mq_open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) {
    printf("open mq failure\n");
    return 1;
  }

  // 通过线程处理
  // setup_notify(&mq);
  // for (;;) {
  //   pause();
  // }

  // 信号处理
  // sigset_t mask;
  // struct sigevent sigev;
  // int sig;
  // int num;
  // mq_getattr(mq, &attr);
  // char *buf = (char *)malloc(attr.mq_msgsize);
  // // 设置信号集
  // sigemptyset(&mask);
  // sigaddset(&mask, SIGUSR1);
  // sigprocmask(SIG_BLOCK, &mask, NULL);

  // sigev.sigev_notify = SIGEV_SIGNAL;  // 使用信号notify
  // sigev.sigev_signo = SIGUSR1;        // 使用信号SIGUSR1
  // mq_notify(mq, &sigev);              // 注册notify

  // for (;;) {
  //   sigwait(&mask, &sig);  // 等待信号
  //   if (sig == SIGUSR1) {
  //     mq_notify(mq, &sigev);
  //     while ((num = mq_receive(mq, buf, attr.mq_msgsize, NULL)) >= 0) {
  //       fprintf(stderr, "receive %d bytes, content: %s", num, buf);
  //     }
  //   }
  // }

  // epoll 处理
  struct epoll_event events[10];
  int epollfd = epoll_create(2);
  add_epoll(epollfd, mq);
  mq_getattr(mq, &attr);
  char *buf = (char *)malloc(attr.mq_msgsize);
  while (1) {
    printf("epoll waiting message\n");
    int ret = epoll_wait(epollfd, events, 10, -1);
    if (ret > 0) {
      int num;
      for (int i = 0; i < ret; ++i) {
        int fd = events[i].data.fd;
        if ((fd == mq) && (events[i].events & EPOLLIN)) {
          while ((num = mq_receive(fd, buf, attr.mq_msgsize, 0)) >= 0) {
            printf("receive %d bytes, content: %s", num, buf);
          }
        }
      }
    } else if (ret < 0) {
      printf("events error: %d\n", errno);
      break;
    }
  }

  close(epollfd);
  close(mq);
  return 0;
}