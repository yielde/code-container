#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s /shmname\n", argv[1]);
    return 1;
  }
  const char *shmname = argv[1];

  int shmfd = shm_open(shmname, O_RDONLY, 0666);
  assert(shmfd != -1);
  char *shmptr;
  struct stat statbuf;
  int ret = fstat(shmfd, &statbuf);
  assert(ret != -1);
  printf("shm length is %ld bytes\n", statbuf.st_size);
  shmptr = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, shmfd,0);
  if (shmptr == MAP_FAILED) {
    printf("map shm failure\n");
    return 1;
  }
  printf("%s", shmptr);
  munmap(shmptr, statbuf.st_size);
  return 0;
}