#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 1024

int setblock(int fd);
int setnonblock(int fd);

int main() {
  char buf[BUFSIZE];
  for (;;) {
    fprintf(stderr, "Blocking input on stdin");
    setblock(STDIN_FILENO);
    int n = read(STDIN_FILENO, buf, BUFSIZE);
    if (n == 0) break;

    if (n > 0)
      fprintf(stderr, "Got %d bytes: %.*s\n", n, n, buf);
    else
      perror("Blocking error");
    fprintf(stderr, "Nonblocking input on stdin");
    n = 0;
    setnonblock(STDIN_FILENO);
    while (n <= 0) {
      n = read(STDIN_FILENO, buf, BUFSIZE);
      if (n > 0)
        fprintf(stderr, "Got %d bytes: %.*s\n", n, n, buf);
      else
        perror("Nonblocking error");
      sleep(1);
    }
  }
  return 0;
}