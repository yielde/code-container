#include <iostream>
#include <map>
#include <string>

using std::map, std::string;

int main() {
  map<string, size_t> word_count;
  string word;
  while (std::cin >> word) ++word_count[word];

  // for (const auto &w : word_count)
  //   std::cout << w.first << " occurs " << w.second
  //             << ((w.second > 1) ? " times" : "time") << std::endl;

  /*
    迭代器读取
  */
  auto map_it = word_count.cbegin();
  while (map_it != word_count.cend()) {
    std::cout << map_it->first << " occurs " << map_it->second
              << ((map_it->second > 1) ? " times" : " time") << std::endl;
    ++map_it;
  }
}