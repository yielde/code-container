#include <netinet/in.h>

#include <ctime>
#include <iostream>
#define BUFFER_SIZE 64

using namespace std;

class util_timer;

struct client_data {
  struct sockaddr_in address;
  int sockfd;
  char buf[BUFFER_SIZE];
  util_timer *timer;
};

class util_timer {
 public:
  util_timer() : prev(NULL), next(NULL) {}
  time_t expire;
  void (*cb_func)(client_data *);
  client_data *user_data;
  util_timer *prev;
  util_timer *next;
};

// 升序、双向链表
class sort_time_lst {
 public:
  sort_time_lst() : head(NULL), tail(NULL) {}
  ~sort_time_lst() {
    util_timer *tmp = head;
    while (tmp) {
      head = tmp->next;
      delete tmp;
      tmp = head;
    }
  }
  void add_timer(util_timer *timer);
  void adjust_timer(util_timer *timer);
  void delete_timer(util_timer *timer);
  void tick();

 private:
  void add_timer(util_timer *timer, util_timer *lst_head);
  util_timer *head;
  util_timer *tail;
};
