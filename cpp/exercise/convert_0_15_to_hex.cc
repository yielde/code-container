#include <iostream>
using std::cin, std::cout, std::endl, std::string;

int main() {
  const string hex = "0123456789ABCDEF";
  unsigned int num;
  string result;
  while (cin >> num) {
    if (num < hex.size()) {
      result += hex[num];
    }
  }
  cout << result << endl;
  return 0;
}