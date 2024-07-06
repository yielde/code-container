#include <cstring>
#include <iostream>
using std::cout, std::endl;

#define BLANK_LINE cout << endl;

#define STRIP_LINE                                       \
  cout << endl;                                          \
  fill_n(std::ostreambuf_iterator<char>(cout), 50, '*'); \
  cout << endl;
