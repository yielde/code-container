#include <string>
inline std::string make_plural(size_t ctr, const std::string &word,
                               const std::string &ending) {
  return (ctr > 1) ? word + ending : word;
}