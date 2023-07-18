#include "my_atomic_logger.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPEN_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

typedef struct list {
  char *message;
  int len;
  struct list *next;
} log_list;

static int fd = -1;
static log_list *first = NULL;
static log_list *last = NULL;

/* 如果write被信号中断，重新执行 */
static ssize_t r_write(int fd, void *buf, size_t size) {
  ssize_t writebytes;
  while (writebytes = write(fd, buf, size), writebytes == -1 && errno == EINTR)
    ;
  return writebytes;
}

/*
  为插入len长度的条目分配地址
  成功：返回指向存放条目的地址指针
  失败：返回NULL
*/
static log_list *allocate_message_space(int len, int extra) {
  log_list *new_entry;
  char *message;

  new_entry = (log_list *)malloc(sizeof(log_list) + len + extra);
  if (new_entry == NULL) return NULL;
  message = (char *)new_entry + sizeof(log_list);
  new_entry->message = message;
  new_entry->len = len;
  new_entry->next = NULL;

  if (last == NULL)
    first = new_entry;
  else
    last->next = new_entry;
  last = new_entry;
  return new_entry;
}

/* 返回list的长度 */
static int get_length() {
  log_list *current_entry;
  int len = 0;
  current_entry = first;
  while (current_entry != NULL) {
    len += current_entry->len;
    current_entry = current_entry->next;
  }
  return len;
}

/* 清空list并释放空间和指针 */
static int clear_list() {
  log_list *current_entry;
  log_list *free_entry;
  current_entry = first;
  while (current_entry != NULL) {
    free_entry = current_entry;
    current_entry = current_entry->next;
    free(free_entry);
  }
  first = NULL;
  last = NULL;
}

/*
  打开日志文件，如果被信号中断则重新打开
  成功：返回0
  失败：返回-1，设置errno
*/

int atomic_log_open(char *fn) {
  while (fd = open(fn, OPEN_FLAGS, FILE_PERMS), fd == -1 && errno == EINTR)
    ;
  if (fd < 0) return -1;
  return 0;
}

/*
  将已知字节数的数组插入到list
  成功：返回0
  失败：返回-1，设置errno
*/
int atomic_log_array(char *s, int len) {
  if (fd < 0) {
    errno = EINVAL;
    return -1;
  }
  log_list *new_entry;
  new_entry = allocate_message_space(len, 0);
  if (new_entry == NULL) return -1;
  (void)memcpy(new_entry->message, s, len);  // 丢弃返回值
  return 0;
}

/*
  插入string到list，不包括string的结束符
  成功：返回0
  失败：返回-1，设置errno
*/
int atomic_log_string(char *s) { atomic_log_array(s, strlen(s)); }

/*
  以类似printf的语法将string插入到list，包括string的结束符，但是不计入len
  成功：返回0
  失败：返回-1，设置errn
*/
int atomic_log_printf(char *fmt, ...) {
  if (fd < 0) {
    errno = EINVAL;
    return -1;
  }
  va_list arg;
  int len;
  log_list *new_entry;

  va_start(arg, fmt);
  len = vsnprintf(0, 0, fmt, arg);
  new_entry = allocate_message_space(len, 1);
  if (new_entry == NULL) return -1;
  va_start(arg, fmt);
  vsprintf(new_entry->message, fmt, arg);
  va_end(arg);
  return 0;
}

/*
  将日志list一次写入到文件，清空list
  成功：返回0
  失败：返回-1，设置errn
*/
int atomic_log_send() {
  if (fd < 0) {
    errno = EINVAL;
    return -1;
  }
  log_list *current_entry;
  char *buf;
  int len = get_length();
  if (len == 0) return 0;

  buf = (char *)malloc(len);
  if (buf == NULL) return -1;
  current_entry = first;
  len = 0;
  while (current_entry != NULL) {
    (void)memcpy(buf + len, current_entry->message, current_entry->len);
    len += current_entry->len;
    current_entry = current_entry->next;
  }
  if (r_write(fd, buf, len) != len) {
    free(buf);
    errno = EAGAIN;
    return -1;
  }
  free(buf);
  clear_list();
  return 0;
}

/*
  清除list，不写入log文件
*/
int atomic_log_clear() {
  clear_list();
  return 0;
}

/*
  关闭日志文件，如果被信号中断则重新关闭
*/
int atomic_log_close() {
  int retval;
  clear_list();
  while (retval = close(fd), retval == -1 && errno == EINTR)
    ;
  return retval;
}