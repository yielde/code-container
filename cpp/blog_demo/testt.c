#include <stdio.h>
#include <unistd.h>
#define COUNT 40
int main() {
  char buf[8] = {0};
  if (buf[0] == 0) {
    printf("hello\n");
  }
  return 0;
}