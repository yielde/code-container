#include <iostream>

using namespace std;

int main() {
  char a = 'a';
  char b = 'b';
  // cout << "a=" << static_cast<void *>(&a) << "b=" << static_cast<void *>(&b)
  //      << " " << endl;
  printf("a:%p b:%p\n", &a, &b);
  char *p = &a;
  char *c = p;
  p = &b;
  printf("p=%p c=%p\n", p, c);
  // cout << "p=" << static_cast<void *>(p) << "c=" << static_cast<void *>(c)
  //      << endl;
  return 0;
}