#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

map<string, string> buildMap(ifstream &mapfile) {
  map<string, string> trans_map;
  string key;
  string value;
  while ((mapfile >> key) && getline(mapfile, value)) {
    if (value.size() > 1)
      trans_map[key] = value.substr(1);
    else
      throw runtime_error("no rule for " + key);
  }
  return trans_map;
}

const string &transform(const string &s, const map<string, string> &m) {
  auto map_it = m.find(s);
  if (map_it != m.cend())
    return map_it->second;
  else
    return s;
}

void word_transform(ifstream &map_file, ifstream &input) {
  auto trans_map = buildMap(map_file);
  string text;
  while (getline(input, text)) {
    istringstream stream(text);
    string word;
    bool firstword = true;
    while (stream >> word) {
      if (firstword)
        firstword = false;
      else
        cout << " ";
      cout << transform(word, trans_map);
    }
    cout << endl;
  }
}