#include "timer_min_heap.h"

void time_heap::add_timer(heap_timer *timer) throw(std::exception) {
  if (!timer) {
    return;
  }
  if (cur_size >= capacity) {
    resize();
  }
  int hole = cur_size++;
  int parent = 0;
  for (; hole > 0; hole = parent) {
    parent = (hole - 1) / 2;
    if (array[parent]->expire <= timer->expire) {
      break;
    }
    array[hole] = array[parent];
  }
  array[hole] = timer;
}

void time_heap::delete_timer(heap_timer *timer) {
  if (!timer) {
    return;
  }
  timer->cb_func = NULL;
}

heap_timer *time_heap::top() const {
  if (empty()) {
    return NULL;
  }
  return array[0];
}

void time_heap::pop_timer() {
  if (empty()) {
    return;
  }
  if (array[0]) {
    array[0] = array[--cur_size];
    percolate_down(0);
  }
}

void time_heap::tick() {
  heap_timer *tmp = array[0];
  time_t cur = time(NULL);
  cout << "tick once" << endl;
  while (!empty()) {
    if (!tmp) {
      break;
    }
    if (tmp->expire > cur) {
      break;
    }
    if (array[0]->cb_func) {
      array[0]->cb_func(array[0]->user_data);
    }
    pop_timer();
    tmp = array[0];
  }
}

bool time_heap::empty() const { return cur_size == 0; }

void time_heap::percolate_down(int hole) {
  heap_timer *tmp = array[hole];
  int child = 0;
  for (; (hole * 2 + 1) <= (cur_size - 1); hole = child) {
    child = hole * 2 + 1;
    if ((child < (cur_size - 1)) &&
        (array[child + 1]->expire < array[child]->expire)) {
      ++child;
    }
    if (array[child]->expire < tmp->expire) {
      array[hole] = array[child];
    } else {
      break;
    }
  }
  array[hole] = tmp;
}

void time_heap::resize() throw(std::exception) {
  heap_timer **tmp = new heap_timer *[2 * capacity];
  for (int i = 0; i < 2 * capacity; ++i) {
    tmp[i] = NULL;
  }
  if (!tmp) {
    throw std::exception();
  }
  capacity = 2 * capacity;
  for (int i = 0; i < cur_size; ++i) {
    tmp[i] = array[i];
  }
  delete[] array;
  array = tmp;
}
