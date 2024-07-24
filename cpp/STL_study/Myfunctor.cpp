#include <algorithm>
#include <functional>
#include <vector>

#include "publiclib.h"

// void func(int i) { cout << i << " "; }

// class Func {
//  public:
//   Func() { cout << "使用仿函数" << endl; }
//   void operator()(int i) { cout << i << " "; }
// };

// int main() {
//   vector<int> vec{1, 2, 3, 4, 5, 6};

//   cout << "使用函数：";
//   for_each(vec.begin(), vec.end(), func);
//   STRIP_LINE
//   for_each(vec.begin(), vec.end(), Func());
//   STRIP_LINE

//   return 0;
// }

void mfunc(int i, int p) {
  if (i > p) {
    cout << i << " ";
  }
}

class mprint {
 public:
  mprint(int m) : max(m) {}
  void operator()(int x) {
    if (x > max) {
      cout << x << " ";
    }
  }

 private:
  int max;
};

template <typename T>
class printor {
 public:
  printor(T m) : max(m) {}

  void operator()(int x) {
    if (x > max) {
      cout << x << " ";
    }
  }

  void operator()(string x) {
    if (x.size() > max) {
      cout << x << " ";
    }
  }

  void operator()(double x) {
    if (x > max) {
      cout << x << " ";
    }
  }

 private:
  T max;
};

int main() {
  vector<int> vec{1, 2, 3, 4, 5, 6};
  // 打印大于3的值
  cout << "函数: " << endl;
  for_each(vec.begin(), vec.end(), bind(mfunc, placeholders::_1, 3));
  STRIP_LINE
  for_each(vec.begin(), vec.end(), mprint(3));
  STRIP_LINE
  cout << "functor ：" << endl;
  cout << "int: " << endl;
  for_each(vec.begin(), vec.end(), printor<int>(3));
  cout << endl;

  cout << "string: " << endl;
  vector<string> vecs{"he",     "hell",    "hello",
                      "hello ", "hello w", "hello world!"};
  for_each(vecs.begin(), vecs.end(), printor<int>(5));
  cout << endl;
  return 0;
}