#include <timer_wheel.h>

#include <iostream>

using namespace std;

tw_timer* time_wheel::add_timer(int timeout) {
  if (timeout < 0) {
    return NULL;
  }
  int ticks = 0;
  if (timeout < SI) {
    ticks = 1;
  } else {
    ticks = timeout / SI;
  }
  int rotation = ticks / N;
  int ts = (cur_slot + (ticks % N)) % N;
  tw_timer* timer = new tw_timer(rotation, ts);
  if (!slots[ts]) {
    cout << "add timer, rotation is " << rotation << ", ts is " << ts
         << ", cur_slot is " << cur_slot << endl;
    slots[ts] = timer;
  } else {
    timer->next = slots[ts];
    slots[ts]->prev = timer;
    slots[ts] = timer;
  }
  return timer;
}

void time_wheel::delete_timer(tw_timer* timer) {
  if (!timer) {
    return;
  }
  int ts = timer->time_slot;
  if (timer == slots[ts]) {
    slots[ts] = slots[ts]->next;
    if (slots[ts]) {
      slots[ts]->prev = NULL;
    }
    delete timer;
  } else {
    timer->prev->next = timer->next;
    if (timer->next) {
      timer->next->prev = timer->prev;
    }
    delete timer;
  }
}

void time_wheel::tick() {
  tw_timer* tmp = slots[cur_slot];
  cout << "current slot is " << cur_slot << endl;
  while (tmp) {
    cout << "tick the timer once" << endl;
    if (tmp->rotation > 0) {
      tmp->rotation--;
      tmp = tmp->next;
    } else {
      tmp->cb_func(tmp->user_data);
      if (tmp == slots[cur_slot]) {
        cout << "delete header in cur_slot" << endl;
        slots[cur_slot] = tmp->next;
        delete tmp;
        if (slots[cur_slot]) {
          slots[cur_slot]->prev = NULL;
        }
        tmp = slots[cur_slot];
      } else {
        tmp->prev->next = tmp->next;
        if (tmp->next) {
          tmp->next->prev = tmp->prev;
        }
        tw_timer* tmp2 = tmp->next;
        delete tmp;
        tmp = tmp2;
      }
    }
  }
  cur_slot = ++cur_slot % N;
}

