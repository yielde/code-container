#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int *p = {1, 2, 3};
  printf("%d", p[2]);
}
