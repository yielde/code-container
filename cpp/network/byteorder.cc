#include <iostream>
using namespace std;
void byteorder() {
  union {
    short value;
    char union_bytes[sizeof(short)];
  } test;
  test.value = 0x0102;
  if ((test.union_bytes[0] == 0x01) && (test.union_bytes[1] == 0x02)) {
    cout << "big endian" << endl;  // [0x01, 0x02]
  } else if ((test.union_bytes[0] == 0x02) && (test.union_bytes[1] == 0x01)) {
    cout << "little endian" << endl;  // [0x02, 0x01]
  } else {
    cout << "unknow~" << endl;
  }
}

int main() { byteorder(); }