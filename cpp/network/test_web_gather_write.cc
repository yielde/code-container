#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define BUFSIZE 1024
using namespace std;

static const char *status_line[2] = {"200 OK", "500 Interal server error"};

int main(int argc, char *argv[]) {
  if (argc <= 3) {
    cout << "usage: " << argv[0] << " ip_address port_number filename" << endl;
    return 1;
  }
  const char *ip = argv[1];
  int port = atoi(argv[2]);
  const char *filename = argv[3];

  struct sockaddr_in address, client_addr;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sockfd, 3);
  assert(ret != -1);
  socklen_t client_addr_len = sizeof(client_addr);
  int connfd =
      accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (connfd < 0) {
    cout << "connect error: " << errno << " " << strerror(errno) << endl;
  } else {
    // HTTP head buff
    char head_buf[BUFSIZE];
    memset(head_buf, '\0', BUFSIZE);
    // context buff
    char *file_buf;

    struct stat file_stat;
    // 有效文件flag
    bool valid = true;
    int len = 0;
    if (stat(filename, &file_stat) < 0) {
      valid = false;
    } else {
      if (S_ISDIR(file_stat.st_mode)) {
        valid = false;
      } else if (file_stat.st_mode & S_IROTH) {
        int fd = open(filename, O_RDONLY);
        // 动态分配堆内存
        file_buf = new char[file_stat.st_size + 1];
        memset(file_buf, '\0', file_stat.st_size + 1);
        if (read(fd, file_buf, file_stat.st_size) < 0) {
          valid = false;
        }
      } else {
        valid = false;
      }
    }

    if (valid) {
      // HTTP头部状态放入head_buf
      ret = snprintf(head_buf, BUFSIZE - 1, "%s %s\r\n", "HTTP/1.1",
                     status_line[0]);
      len += ret;
      ret = snprintf(head_buf + len, BUFSIZE - 1 - len,
                     "Content-Length: %ld\r\n", file_stat.st_size);
      len += ret;
      ret = snprintf(head_buf + len, BUFSIZ - 1 - len, "%s", "\r\n");
      // 聚集写
      struct iovec iv[2];
      iv[0].iov_base = head_buf;
      iv[0].iov_len = strlen(head_buf);
      iv[1].iov_base = file_buf;
      iv[1].iov_len = file_stat.st_size;

      ret = writev(connfd, iv, 2);
    } else {
      ret = snprintf(head_buf, BUFSIZE - 1, "%s %s\r\n", "HTTP/1.1",
                     status_line[1]);
      len += ret;
      ret = snprintf(head_buf + len, BUFSIZE - 1 - len, "%s", "\r\n");
      send(connfd, head_buf, strlen(head_buf), 0);
    }
    close(connfd);
    delete[] file_buf;
  }
  close(sockfd);
  return 0;
}