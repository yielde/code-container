#include <string>
#include <vector>

#include "publiclib.h"

void print(vector<string> &p) {
  for (auto it : p) {
    cout << it << " ";
  }
  cout << endl;
}

int main() {
  vector<string> s;
  s.push_back("abcdef");
  print(s);
  STRIP_LINE

  string pp("pppp");
  s.push_back(pp);
  print(s);
  cout << pp << endl;

  s.push_back(move(pp));
  print(s);
  cout << "pp: " << pp << endl;

  STRIP_LINE

  s.insert(s.begin(), 3, "hello");
  print(s);
  STRIP_LINE

  vector<string> tmp{"a", "b", "c", "d"};
  vector<string>::iterator vit = tmp.begin();
  s.insert(s.end() - 3, tmp.begin() + 1, tmp.end() - 1);
  print(s);
  STRIP_LINE

  s.erase(s.begin() + 3, s.begin() + 6);
  print(s);
  STRIP_LINE
  print(tmp);
  print(s);
  cout << "tmp: " << tmp.capacity() << "s: " << s.capacity() << endl;
  STRIP_LINE
  swap(tmp, s);
  print(tmp);
  print(s);
  cout << "tmp: " << tmp.capacity() << "s: " << s.capacity() << endl;
  STRIP_LINE
  print(s);
  sort(s.begin(), s.end(), greater<string>());
  print(s);
  STRIP_LINE
  s.resize(2);
  print(s);
  return 0;
}