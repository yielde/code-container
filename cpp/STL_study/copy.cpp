#include <algorithm>
#include <iterator>
#include <vector>

#include "publiclib.h"

int main() {
  vector<char> myvec;
  cout << "input :" << endl;

  copy(istream_iterator<char>(cin), istream_iterator<char>(),
       back_insert_iterator<vector<char>>(myvec));

  cout << endl << "over" << endl;

  cout << "output :" << endl;

  copy(myvec.begin(), myvec.end(), ostream_iterator<char>(cout, "\t"));

  cout << endl;
  STRIP_LINE
  vector<char> newv(myvec.size() + 1);
  newv = {'a', 'b', 'c'};
  print(myvec);
  newv.resize(myvec.size());
  print(newv);
  cout << newv.size() << endl;
  copy_backward(myvec.begin(), myvec.end(), newv.end());
  print(newv);
  return 0;
}