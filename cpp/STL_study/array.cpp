#include <algorithm>
#include <array>
#include <iostream>
using namespace std;

void show(array<int, 6> &ar) {
  auto it = ar.begin();
  while (it != ar.end()) {
    cout << *it;
    ++it;
  }
  cout << endl;
}

int main() {
  array<int, 6> arr{1, 3, 4, 2, 5};

  show(arr);

  sort(arr.begin(), arr.end());
  show(arr);

  cout << arr.at(2) << endl;

  auto idx = get<4>(arr);
  cout << idx << endl;
  cout << *(arr.data()) << endl;

  cout << arr.size() << ": " << arr.max_size() << endl;

  auto print = [](int n) { cout << " " << n; };

  for_each(arr.begin(), arr.end(), print);
  return 0;
}