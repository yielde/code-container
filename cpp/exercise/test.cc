#include <iostream>
using std::cin, std::cout, std::string, std::endl;

int sun(int a, int b) {
  int result;
  a = 2;
  b = 3;
  result = a + b;
  return result;
}

int main() {
  const int c = 1;
  const int d = 2;
  int a;

  a = sun(c, d);

  cout << a << endl;
  cout << c << " " << d << endl;
  return 0;
}