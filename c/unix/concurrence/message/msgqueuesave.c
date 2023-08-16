#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../../restart.h"
#include "msgqueuelog.h"

#define MAXSIZE 4096

typedef struct {
  long mtype;
  char mtext[MAXSIZE];
} mymsg_t;

int main(int argc, char *argv[]) {
  int id, key, size;
  mymsg_t mymsg;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s key\n", argv[0]);
    return 1;
  }

  key = atoi(argv[1]);
  if ((id = initqueue(key)) == -1) {
    perror("Failed to initialize message queue");
    return 1;
  }
  for (;;) {
    if ((size = msgread(&mymsg, MAXSIZE)) == -1) {
      perror("Failed to read message queue");
      break;
    }
    if (r_write(STDOUT_FILENO, mymsg.mtext, size) == -1) {
      perror("Failed to write to standard output");
      break;
    }
  }
  return 1;
}