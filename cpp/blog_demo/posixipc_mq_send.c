#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define OFLAG (O_CREAT | O_EXCL | O_WRONLY)
#define PERM (S_IRUSR | S_IWUSR)

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s /mqname\n", argv[0]);
    return 1;
  }
  const char *mqname = argv[1];
  mqd_t mq = mq_open(mqname, OFLAG, PERM, NULL);
  struct mq_attr attr;
  mq_getattr(mq, &attr);
  char *buf = (char *)malloc(attr.mq_msgsize);

  while ((fgets(buf, attr.mq_msgsize, stdin) != NULL) && (buf[0] != '\n')) {
    mq_send(mq, buf, attr.mq_msgsize, 0);
  };
  close(mq);
  return 0;
}