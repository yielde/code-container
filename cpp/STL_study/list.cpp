#include <list>

#include "publiclib.h"

int main() {
  list<int> l1{5, 6, 3, 2, 1};
  list<int> l2;
  for (int i = 0; i < 5; ++i) {
    l2.push_back(i);
  }
  print(l1);
  print(l2);
  STRIP_LINE

  cout << "merge" << endl;
  l1.sort();
  print(l1);
  print(l2);

  l1.merge(l2);

  print(l1);
  cout << "l2: " << l2.size();
  print(l2);
  STRIP_LINE

  list<int> l3{100, 200, 300};
  print(l1);
  print(l3);
  auto it = l1.begin();
  std::advance(it, 3);
  auto l3it = l3.end();
  std::advance(l3it, -1);
  l1.splice(it, l3, l3.begin(), l3it);

  print(l1);
  print(l3);
  STRIP_LINE
  print(l1);
  l1.unique();
  print(l1);
  STRIP_LINE

  list<int> l4{1, 2, 2, 2, 3, 4};
  l4.unique();
  print(l4);
  STRIP_LINE
  l4.emplace_back(4);
  l4.push_back(4);
  print(l4);
  l4.pop_front();
  print(l4);
}