#include <iostream>
#include <map>
#include <set>
#include <vector>
using std::cin, std::cout, std::endl;
using std::multimap, std::multiset, std::map, std::set;

int main() {
  std::vector<int> ivec;

  for (int i = 0; i < 10; ++i) {
    ivec.push_back(i);
    ivec.push_back(i);
  }

  set<int> iset(ivec.cbegin(), ivec.cend());
  multiset<int> miset(ivec.cbegin(), ivec.cend());

  cout << ivec.size() << endl;
  cout << iset.size() << endl;
  cout << miset.size() << endl;

  // cout << ivec.size() << endl;
  // for (auto i : ivec) {
  //   cout << i << " ";
  // }
  // cout << endl;
}