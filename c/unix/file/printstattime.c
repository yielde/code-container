#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define CTIME_SIZE 26

void printacessmod(char *path) {
  char atime[CTIME_SIZE];
  struct stat statbuf;
  if (stat(path, &statbuf) == -1)
    perror("Failed to get file status");
  else {
    strncpy(atime, ctime(&statbuf.st_atime), CTIME_SIZE - 1);
    atime[CTIME_SIZE - 2] = 0;
    printf("%s accessd: %s modified: %s", path, atime,
           ctime(&statbuf.st_mtime));
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage:%s filename\n", argv[0]);
    return 1;
  }
  printacessmod(argv[1]);
} 