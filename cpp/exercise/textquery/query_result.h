#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

class QueryResult {
  friend std::ostream& print(std::ostream&, const QueryResult&);

 public:
  typedef std::vector<std::string>::size_type line_no;
  QueryResult(std::string s, std::shared_ptr<std::set<line_no>> p,
              std::shared_ptr<std::vector<std::string>> f)
      : sought(s), lines(p), file(f) {}

 private:
  std::string sought;
  std::shared_ptr<std::set<line_no>> lines;
  std::shared_ptr<std::vector<std::string>> file;
};
