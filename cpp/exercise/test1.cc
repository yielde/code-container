#include <iostream>
#include <vector>

int main() {
  int j = 0;
  int q;
  for (int i = 0; i < 10; ++i) {
    std::cout <<"i: " << i;
    q = ++j;
    std::cout << " q: " << q << std::endl;
    break;
  }
  std::cout << " j: " << j << std::endl;
}
