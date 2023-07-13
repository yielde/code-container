#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define BLKSIZE 1024

int copyfile(int fromfd, int tofd) {
  int readbytes, writtenbytes;
  int totalbytes = 0;
  char buf[BLKSIZE];
  char *bp;

  for (;;) {
    while (((readbytes = read(fromfd, buf, BLKSIZE)) == -1) && (errno == EINTR))
      ;
    if (readbytes <= 0) break;
    bp = buf;
    while (readbytes > 0) {
      while (((writtenbytes = write(tofd, bp, readbytes)) == -1) &&
             (errno == EINTR))
        ;
      if (writtenbytes <= 0) break;
      totalbytes += writtenbytes;
      readbytes -= writtenbytes;
      bp += writtenbytes;
    }
    if (writtenbytes == -1) break;
  }
  return totalbytes;
}

int main() {
  int numbytes;
  numbytes = copyfile(STDIN_FILENO, STDOUT_FILENO);
  fprintf(stderr, "Number of bytes copied: %d\n", numbytes);
  return 0;
}