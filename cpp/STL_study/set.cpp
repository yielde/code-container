#include <set>

#include "publiclib.h"
class P {
 public:
  P(string s, int a) : name(s), age(a) {}

  bool operator<(const P &p) const {
    if (name == p.name) {
      return age < p.age;
    }
    return name < p.name;
  }
  void operator()() const { cout << name << " : " << age << endl; }

 private:
  string name;
  int age;
};

int main() {
  set<int> s;
  s.insert(8);
  s.insert(6);
  auto tt = s.insert(2);
  s.insert(3);
  auto it = s.insert(7);

  print(s);
  cout << *it.first << ": " << it.second << endl;
  STRIP_LINE
  auto it1 = s.insert(2);
  if (it1.first == tt.first) {
    cout << *it1.first << ": " << it1.second << endl;
    cout << *tt.first << ": " << tt.second << endl;
  }
  STRIP_LINE
  print(s);

  auto it2 = s.begin();
  it2++;
  it2++;
  cout << *it2 << endl;

  s.insert(it2, 5);
  print(s);
  STRIP_LINE

  size_t r = s.erase(4);
  cout << r << endl;
  r = s.erase(5);
  cout << r << endl;
  print(s);

  auto ttt2 = s.upper_bound(6);
  cout << *ttt2 << endl;
  auto ttt1 = s.lower_bound(3);
  cout << *ttt1 << endl;

  s.erase(ttt1, ttt2);
  print(s);
  STRIP_LINE

  P p1("galen1", 11);
  P p2("galen2", 11);
  P p3("galen3", 11);
  P p4("galen1", 11);
  P p5("galen1", 13);

  set<P> myp;
  myp.insert(p1);
  myp.insert(p2);
  myp.insert(p3);
  myp.insert(p4);
  myp.insert(p5);

  for (auto i : myp) {
    i();
  }
STRIP_LINE
  P mypp("galen1", 11);
  auto ittt = myp.find(mypp);
  if (ittt == myp.end()) {
    cout << "no" << endl;
  } else {
    ittt->operator()();
  }

  return 0;
}