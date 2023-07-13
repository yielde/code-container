#include <sys/types.h>
int r_wait(int *stat_local);
ssize_t r_read(int fd, void *buf, size_t size);
ssize_t r_write(int fd, void *buf, size_t size);