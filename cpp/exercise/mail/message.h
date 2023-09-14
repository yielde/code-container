#ifndef MESSAGE_H
#define MESSAGE_H
#include <memory>
#include <set>
#include <string>

class Folder;
class Message {
  friend class Folder;
  friend void swap(Message &, Message &);

 public:
  explicit Message(const std::string &str = "") : contents(str){};
  Message(const Message &);
  Message &operator=(const Message &);
  ~Message();
  void save(Folder &);
  void remove(Folder &);

 private:
  std::string contents;
  std::set<Folder *> folders;
  void add_to_Folders(const Message &);
  void remove_from_Folders();
};

class Folder {
  friend class Message;
  friend void swap(Message &, Message &);

 public:
  ~Folder();
  Folder(const Folder &);
  Folder &operator=(Folder &);
  Folder(Folder &&);
  Folder &operator=(Folder &&);

  Folder() = default;
  void save(Message &);
  void remove(Message &);

  void debug_print();

 private:
  std::set<Message *> msgs;
  void addMsg(Message *m) { msgs.insert(m); }
  void remMsg(Message *m) { msgs.erase(m); }
};

void Message::save(Folder &f) {
  folders.insert(&f);
  f.addMsg(this);
}

void Message::remove(Folder &f) {
  folders.erase(&f);
  f.remMsg(this);
}

void Message::add_to_Folders(const Message &m) {
  for (auto f : m.folders) f->addMsg(this);
}
// 拷贝构造函数
Message::Message(const Message &m) : contents(m.contents), folders(m.folders) {
  add_to_Folders(m);
}

void Message::remove_from_Folders() {
  for (auto f : folders) f->remMsg(this);
}

Message::~Message() { remove_from_Folders(); }

Message &Message::operator=(const Message &rhs) {
  remove_from_Folders();
  contents = rhs.contents;
  folders = rhs.folders;
  add_to_Folders(rhs);
  return *this;
}

void swap(Message &lhs, Message &rhs) {
  using std::swap;
  for (auto f : lhs.folders) f->remMsg(&lhs);
  for (auto f : rhs.folders) f->remMsg(&rhs);

  swap(lhs.folders, rhs.folders);
  swap(lhs.contents, rhs.contents);

  for (auto f : lhs.folders) f->addMsg(&lhs);
  for (auto f : rhs.folders) f->addMsg(&rhs);
}

#endif