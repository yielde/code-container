#include <sys/types.h>
int r_wait(int *stat_local);
ssize_t r_read(int fd, void *buf, size_t size);
ssize_t r_write(int fd, void *buf, size_t size);
int r_close(int fd);
int readwrite(int fromfd, int tofd);
int copyfile(int fromfd, int tofd);
static int gettimeout(struct timeval end, struct timeval *timeoutp);
struct timeval add2currenttime(double seconds);
int waitfdtimed(int fd, struct timeval end);
ssize_t readtimed(int fd, void *buf, size_t nbyte, double seconds);