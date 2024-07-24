#include <unordered_map>

#include "publiclib.h"

class MyObj {
 public:
  MyObj(string n, int a) : name(n), age(a) {}
  struct MyHash {
    size_t operator()(const MyObj& obj) const {
      return hash<string>()(obj.name) ^ (hash<int>()(obj.age) << 1);
    }
  };

  MyObj(const MyObj& obj) : name(obj.name), age(obj.age + 1) {}

  bool operator==(const MyObj& obj) const {
    return obj.name == name && obj.age == age;
  }

  void operator()() const { cout << name << ":" << age; }

 private:
  string name;
  int age;
};

int main() {
  unordered_multimap<string, int> rawmap;

  rawmap.emplace("galen1", 11);
  rawmap.insert({"galen1", 11});
  rawmap.insert(make_pair("galen1", 11));
  rawmap.insert(make_pair("galen2", 11));

  for (auto it : rawmap) {
    cout << it.first << " : " << it.second << endl;
  }

  STRIP_LINE
  unordered_multimap<string, int> torawmap;

  torawmap = rawmap;

  cout << "copy" << endl;
  for (auto it : torawmap) {
    cout << it.first << " : " << it.second << endl;
  }
  STRIP_LINE
  MyObj m1("galen1", 11);
  MyObj m2("galen2", 12);
  MyObj m3("galen3", 13);

  unordered_multimap<MyObj, int, MyObj::MyHash> MyMap;

  MyMap.emplace(m1, 1);
  MyMap.emplace(m1, 2);
  MyMap.emplace(m2, 3);
  MyMap.emplace(m3, 4);
  MyMap.emplace(MyObj("galen2", 15), 5);
  MyMap.insert({MyObj("galen3", 16), 6});
  MyMap.insert(make_pair(MyObj("galen4", 17), 7));
  cout << MyMap.size() << endl;

  for (auto myit : MyMap) {
    myit.first.operator()();
    cout << " - " << myit.second;
    cout << endl;
  }
  STRIP_LINE

  unordered_map<MyObj, int, MyObj::MyHash> toMap;

  copy(MyMap.begin(), MyMap.end(), inserter(toMap, toMap.begin()));

  for (auto toit : toMap) {
    toit.first.operator()();
    cout << " - " << toit.second;
    cout << endl;
  }

  return 0;
}