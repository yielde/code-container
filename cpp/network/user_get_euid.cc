#include <unistd.h>

#include <iostream>

using namespace std;

// 该文件所有者为root:root
// chmod +s user_get_euid
// 切换普通用户运行该文件，uid为运行该程序的用户ID，euid为root ID
int main(int argc, char* argv[]) {
  uid_t uid = getuid();
  uid_t euid = geteuid();
  cout << "userid is: " << uid << " effective userid is: " << euid << endl;
  sleep(5);
  return 0;
}