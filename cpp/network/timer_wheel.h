#include <netinet/in.h>
#include <time.h>

#include <iostream>

#define BUFFER_SIZE 64

class tw_timer;

struct client_data {
  struct sockaddr_in address;
  int sockfd;
  char buf[BUFFER_SIZE];
  tw_timer *timer;
};

class tw_timer {
 public:
  tw_timer(int rot, int ts)
      : next(NULL), prev(NULL), rotation(rot), time_slot(ts) {}

 public:
  int rotation;
  int time_slot;
  void (*cb_func)(client_data *);
  client_data *user_data;
  tw_timer *next;
  tw_timer *prev;
};

class time_wheel {
 public:
  time_wheel() : cur_slot(0) {
    for (int i = 0; i < N; ++i) {
      slots[i] = NULL;
    }
  }
  ~time_wheel() {
    for (int i = 0; i < N; ++i) {
      tw_timer *tmp = slots[i];
      while (tmp) {
        slots[i] = tmp->next;
        delete tmp;
        tmp = slots[i];
      }
    }
  }
  tw_timer *add_timer(int timeout);
  void delete_timer(tw_timer *timer);
  void tick();

 private:
  static const int N = 60;
  static const int SI = 1;
  tw_timer *slots[N];
  int cur_slot;
};