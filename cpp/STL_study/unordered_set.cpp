#include <string>
#include <unordered_set>

#include "publiclib.h"

class P {
 public:
  P(string s, int a) : name(s), age(a) {}
  void operator()() const { cout << name << " : " << age << endl; }

  struct PHash {
    size_t operator()(const P &p) const {
      return hash<string>()(p.name) ^ (hash<int>()(p.age));
    }
  };

  // struct PEqual {
  //   bool operator()(const P &lhs, const P &rhs) const {
  //     return lhs.name == rhs.name && lhs.age == rhs.age;
  //   }
  // };

  bool operator==(const P &rhs) const {
    return name == rhs.name && age == rhs.age;
  }

 public:
  string name;
  int age;
};

int main() {
  P p1("galen1", 11);
  P p2("galen1", 11);
  P p3("galen2", 11);
  P p4("galen3", 12);
  P p5("galen2", 11);

  unordered_set<int> myiset;

  myiset.insert(6);
  myiset.insert(6);
  myiset.insert(5);

  print(myiset);
  STRIP_LINE

  unordered_set<P, P::PHash> myset;

  myset.insert(P("galen1", 1));
  myset.insert(P("galen1", 1));
  myset.insert(P("galen1", 3));
  myset.insert(P("galen1", 4));
  cout << myset.size() << endl;

  for (auto it : myset) {
    it.operator()();
  }
  STRIP_LINE
  auto Find = myset.find(P("galen2", 1));
  Find = myset.find(P("galen1", 1));

  if (Find == myset.end()) {
    cout << "no" << endl;
  } else {
    Find->operator()();
  }
  return 0;
}