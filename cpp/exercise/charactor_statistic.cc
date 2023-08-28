#include <iostream>
using std::string, std::cin, std::cout, std::endl;

string::size_type find_char(const string &s, char c,
                            string::size_type &occurs) {
  auto ret = s.size();
  occurs = 0;
  for (decltype(ret) i = 0; i != s.size(); ++i) {
    if (s[i] == c) {
      if (ret == s.size()) ret = i + 1;
      ++occurs;
    }
  }
  return ret;
}

int main() {
  string::size_type ctr;
  string s = "hello world!";
  char ch = 'o';

  string::size_type result = find_char(s, ch, ctr);

  cout << "position: " << result << " times: " << ctr << endl;
  return 0;
}