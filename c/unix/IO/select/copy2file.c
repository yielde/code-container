#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../restart.h"

int copy2files(int fromfd1, int tofd1, int fromfd2, int tofd2) {
  int bytesread;
  int maxfd;
  int num;
  fd_set readset;
  int totalbytes = 0;

  if ((fromfd1 < 0) || (fromfd1 >= FD_SETSIZE) || (tofd1 < 0) ||
      (tofd1 >= FD_SETSIZE) || (fromfd2 < 0) || (fromfd2 >= FD_SETSIZE) ||
      (tofd2 < 0) || (tofd2 >= FD_SETSIZE))
    return 0;
  maxfd = fromfd1;
  if (maxfd < fromfd2) maxfd = fromfd2;

  for (;;) {
    FD_ZERO(&readset);
    FD_SET(fromfd1, &readset);
    FD_SET(fromfd2, &readset);

    if (((num = select(maxfd + 1, &readset, NULL, NULL, NULL)) == -1) &&
        (errno == EINTR))
      continue;
    if (num == -1) return totalbytes;
    if (FD_ISSET(fromfd1, &readset)) {
      bytesread = readwrite(fromfd1, tofd1);
      if (bytesread < 0) break;
      totalbytes += bytesread;
    }
    if (FD_ISSET(fromfd2, &readset)) {
      bytesread = readwrite(fromfd2, tofd2);
      if (bytesread <= 0) break;
      totalbytes += bytesread;
    }
  }
  return totalbytes;
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    fprintf(stderr, "Usage: %s from_file_1 to_file_1 from_file_2 to_file_2 \n",
            argv[0]);
    return 1;
  }
  int fromfd1, fromfd2, tofd1, tofd2;
  int totalbytes = 0;
  fromfd1 = open(argv[1], O_RDONLY);
  fromfd2 = open(argv[3], O_RDONLY);

  tofd1 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR);
  tofd2 = open(argv[4], O_WRONLY | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR);

  totalbytes = copy2files(fromfd1, tofd1, fromfd2, tofd2);
  printf("total bytes: %d", totalbytes);
  return 0;
}