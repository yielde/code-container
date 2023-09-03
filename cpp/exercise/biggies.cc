#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using std::cout, std::endl;
using std::vector, std::string;

void elimDups(vector<string> &words) {
  std::sort(words.begin(), words.end());
  auto end_unique = std::unique(words.begin(), words.end());
  words.erase(end_unique, words.end());
}

string make_plural(size_t ctr, const string &word, const string &ending) {
  return (ctr > 1) ? word + ending : word;
}
void biggies(vector<string> &words, vector<string>::size_type sz) {
  elimDups(words);  // words按照字典排序，删除重复单词

  // 按照长度排序，长度相同维持字典顺序
  std::stable_sort(
      words.begin(), words.end(),
      [](const string &a, const string &b) { return a.size() < b.size(); });

  // 获取的迭代器指向第一个满足size()>=sz的元素
  auto words_count =
      std::find_if(words.begin(), words.end(),
                   [sz](const string &a) { return a.size() >= sz; });

  auto count = words.end() - words_count;
  cout << count << " " << make_plural(count, "word", "s") << " of length " << sz
       << " or longer" << endl;

  // 打印长度大于等于给定值的单词
  std::for_each(words_count, words.end(),
                [](const string &s) { cout << s << " "; });
  cout << endl;
}