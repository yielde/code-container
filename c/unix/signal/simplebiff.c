#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define MAILFILE "/var/mail/ostudent"
#define SPLEEPTIME 10

int main() {
  int mailfd;
  for (;;) {
    if ((mailfd = open(MAILFILE, O_RDONLY)) != -1) {
      fprintf(stderr, "%s", "\007");
      while ((close(mailfd) == -1) && (errno == EINTR))
        ;
      sleep(SPLEEPTIME);
    }
  }
}