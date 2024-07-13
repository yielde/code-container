#include "publiclib.h"
template <typename T>
struct StackNode {
  T data;
  StackNode<T> *next;
};

template <typename T>
class LinkStack {
 public:
  LinkStack() {
    m_top = nullptr;
    m_length = 0;
  }
  ~LinkStack() {
    StackNode<T> *tmp = m_top;
    while (m_length > 0) {
      m_top = m_top->next;
      delete tmp;
      tmp = m_top;
      --m_length;
    }
    cout << "栈析构~" << endl;
  }
  bool Push(const T &v) {
    StackNode<T> *newNode = new StackNode<T>;
    newNode->data = v;
    newNode->next = m_top;
    m_top = newNode;
    m_length += 1;
    // cout << "插入元素 " << v << endl;
    return true;
  }
  bool Pop(T &v) {
    if (IsEmpty()) {
      cout << "空栈" << endl;
      return false;
    }
    StackNode<T> *delNode = m_top;
    m_top = m_top->next;
    v = delNode->data;
    // cout << "弹出元素 " << v << endl;
    delete delNode;
    --m_length;
    return true;
  }
  bool GetTop(T &v) {
    if (IsEmpty()) {
      cout << "空栈" << endl;
      return false;
    }
    v = m_top->data;
    return true;
  }

  void DispList() {
    if (IsEmpty()) {
      cout << "空栈" << endl;
      return;
    }
    StackNode<T> *tmp = m_top;
    cout << "栈顶开始的元素有：";
    while (tmp != nullptr) {
      cout << tmp->data << " ";
      tmp = tmp->next;
    }
    cout << endl;
  }
  int StackSize() { return m_length; }
  bool IsEmpty() { return m_length == 0; }

 private:
  StackNode<T> *m_top;
  int m_length;
};