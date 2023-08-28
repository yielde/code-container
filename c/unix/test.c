#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int num(int a, int b) {
  int result;
  result = a + b;
  return result;
}

int main() {
  int a;
  a = num(1, 2);
  printf("%d\n", a);
}
