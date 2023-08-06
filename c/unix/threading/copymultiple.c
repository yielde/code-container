
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 80
#define R_FLAGS O_RDONLY
#define W_FLAGS (O_WRONLY | O_CREAT)
#define W_PERMS (S_IWUSR | S_IRUSR)

typedef struct {
  int args[3];
  pthread_t tid;
} copy_t;

void *copyfilepass(void *arg);

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s infile outfile copieds\n", argv[0]);
    return 1;
  }

  int numcopiers;
  copy_t *copies;
  char filename[MAXNAME];
  int error;
  int *bytesp;
  int totalbytes = 0;

  numcopiers = atoi(argv[3]);
  if ((copies = (copy_t *)calloc(numcopiers, sizeof(copy_t))) == NULL) {
    perror("Failed to allocate copier space");
    return 1;
  }

  for (int i = 0; i < numcopiers; i++) {
    copies[i].tid = pthread_self();
    if (snprintf(filename, MAXNAME, "%s.%d", argv[1], i + 1) == MAXNAME) {
      fprintf(stderr, "Input filename %s.%d too long", argv[1], i + 1);
      continue;
    }

    if ((copies[i].args[0] = open(filename, R_FLAGS)) == -1) {
      fprintf(stderr, "Failed to open source file %s: %s\n", filename,
              strerror(errno));
      continue;
    }

    if (snprintf(filename, MAXNAME, "%s.%d", argv[2], i + 1) == MAXNAME) {
      fprintf(stderr, "Output filename %s.%d too long", argv[2], i + 1);
      continue;
    }

    if ((copies[i].args[1] = open(filename, W_FLAGS, W_PERMS)) == -1) {
      fprintf(stderr, "Failed to open dest file %s:%s\n", filename,
              strerror(errno));
      continue;
    }

    if (error = pthread_create(&copies[i].tid, NULL, copyfilepass,
                               copies[i].args)) {
      fprintf(stderr, "Failed to create thread %d: %s\n", i + 1,
              strerror(error));
      copies[i].tid = pthread_self();
    }
  }

  for (int i = 0; i < numcopiers; i++) {
    if (pthread_equal(copies[i].tid, pthread_self())) continue;
    if (error = pthread_join(copies[i].tid, (void **)&bytesp)) {
      fprintf(stderr, "Failed to join thread %d: %s\n", i, strerror(error));
      continue;
    }
    if (bytesp == NULL) {
      fprintf(stderr, "Thread %d failed to return status\n", i);
      continue;
    }

    printf("Thread %d copied %d bytes from %s.%d to %s.%d\n", i, *bytesp,
           argv[1], i + 1, argv[2], i + 1);
    totalbytes += *bytesp;
  }
  printf("Total bytes copied = %d\n", totalbytes);
  return 0;
}