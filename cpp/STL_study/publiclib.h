#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

#define STRIP_LINE                               \
  cout << endl;                                  \
  fill_n(ostream_iterator<char>(cout), 50, '*'); \
  cout << endl;

template <typename T>
void print(T& t) {
  for (auto it : t) {
    cout << it << " ";
  }
  cout << endl;
}