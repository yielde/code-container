#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>

#include <cassert>
#include <cstring>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 1024
static int connfd;

void sig_urg(int sig) {
  int save_errno = errno;
  char buffer[BUFFER_SIZE];
  memset(buffer, '\0', BUFFER_SIZE);
  int ret = recv(connfd, buffer, BUFFER_SIZE - 1, MSG_OOB);
  cout << "got " << ret << " bytes of oob data " << buffer << endl;
  errno = save_errno;
}

void addsig(int sig, void (*sig_handler)(int)) {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = sig_handler;
  sa.sa_flags |= SA_RESTART;
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    cout << "usage: " << argv[0] << " ip port" << endl;
    return 1;
  }

  const char* ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_addr.sin_addr);

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  int ret = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  assert(ret != -1);
  ret = listen(listenfd, 5);
  assert(ret != -1);
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
  if (connfd < 0) {
    cout << "socket accept error, errno: " << errno << endl;
  } else {
    addsig(SIGURG, sig_urg);
    fcntl(connfd, F_SETOWN, getpid());
    char buffer[BUFFER_SIZE];
    while (1) {
      memset(buffer, '\0', BUFFER_SIZE);
      ret = recv(connfd, buffer, BUFFER_SIZE - 1, 0);
      if (ret <= 0) {
        break;
      }
      cout << "got " << ret << "bytes of normal data " << buffer << endl;
    }
    close(connfd);
  }
  close(listenfd);
  return 0;
}