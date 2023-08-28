#include <iostream>
#include <string>

class Screen {
 public:
  typedef std::string::size_type pos;
  Screen() = default;
  Screen(pos ht, pos wd, char c)
      : height(ht), width(wd), contents(ht * wd, ' ') {}
  friend class Window_mgr;
  Screen(pos ht = 0, pos wd = 0)
      : cursor(0), height(ht), width(wd), contents(ht * wd, ' ') {}
  char get() const { return contents[cursor]; }
  inline char get(pos ht, pos wd) const;
  Screen &clear(char bkground);

 private:
  static const char bkground = ' ';

 public:
  Screen &move(pos r, pos c);
  Screen &set(char);
  Screen &set(pos, pos, char);

  Screen &display(std::ostream &os) {
    do_display(os);
    return *this;
  }

  const Screen &display(std::ostream &os) const {
    do_display(os);
    return *this;
  }

 private:
  void do_display(std::ostream &os) const { os << contents; }
  pos cursor = 0;
  pos height = 0, width = 0;
  std::string contents;
};

char Screen::get(pos r, pos c) const {
  pos row = r * width;
  return contents[row + c];
}

inline Screen &Screen::clear(char c) {
  contents = std::string(height * width, c);
  return *this;
}

inline Screen &Screen::move(pos r, pos c) {
  pos row = r * width;
  cursor = row + c;
  return *this;
}

inline Screen &Screen::set(char c) {
  contents[cursor] = c;
  return *this;
}

inline Screen &Screen::set(pos r, pos col, char ch) {
  contents[r * width + col] = ch;
  return *this;
}