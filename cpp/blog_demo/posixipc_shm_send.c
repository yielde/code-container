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
  int shmfd = shm_open(shmname, O_CREAT | O_EXCL | O_RDWR | O_TRUNC,
                       0666);  // 创建共享内存
  assert(shmfd != -1);
  if (ftruncate(shmfd, 1025) == -1) {  // 设置共享内存大小
    printf("resize shm failure\n");
    shm_unlink(shmname);
    return 1;
  }
  int ret;
  struct stat statbuf;
  ret = fstat(shmfd, &statbuf);  // 获取空闲内存大小
  assert(ret != -1);
  printf("shm length is %ld bytes\n", statbuf.st_size);
  char *shmptr;
  shmptr = (char *)mmap(NULL, statbuf.st_size, PROT_WRITE, MAP_SHARED, shmfd,
                        0);  // 通过mmap映射共享内存
  if (shmptr == MAP_FAILED) {
    printf("map shm failure\n");
    shm_unlink(shmname);
    return 1;
  }
  sprintf(shmptr, "%s", "hello world\n");
  sprintf(shmptr + 12, "%s", "hi\n");
  munmap(shmptr, statbuf.st_size);
  return 0;
}