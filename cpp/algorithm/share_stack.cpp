#include <iostream>
using std::cout, std::endl;
#define BLANK_LEN cout << endl;
#define STACK_SIZE 24

enum STACK_TYPE : int { STACK1 = 1, STACK2 = 2 };

template <typename T>
class ShareStack {
 public:
  ShareStack();
  ~ShareStack();

 public:
  bool Pop(const int m_stack_type, T &value);
  bool Push(const int m_stack_type, const T &value);
  void Clear();
  void CurrStackSize() {
    cout << "stack1: " << m_stack_1 << " stack2: " << m_stack_2 << endl;
  }

 private:
  bool isFull();

 private:
  T *m_stack_data;
  int m_stack_max_size;
  int m_stack_1;
  int m_stack_2;
};

template <typename T>
ShareStack<T>::ShareStack() {
  m_stack_max_size = STACK_SIZE;
  m_stack_data = new T[m_stack_max_size];
  m_stack_1 = -1;
  m_stack_2 = m_stack_max_size;
}

template <typename T>
ShareStack<T>::~ShareStack() {
  delete[] m_stack_data;
  cout << "析构" << endl;
}

template <typename T>
bool ShareStack<T>::Pop(const int m_stack_type, T &value) {
  if (m_stack_type == STACK1) {
    if (m_stack_1 > -1) {
      {
        value = m_stack_data[m_stack_1--];
        return true;
      }
    }
  } else if (m_stack_type == STACK2) {
    if (m_stack_2 < m_stack_max_size) {
      value = m_stack_data[m_stack_2++];
      return true;
    }
  } else {
    cout << "请指定存放在哪个栈" << endl;
  }
  return false;
}

template <typename T>
bool ShareStack<T>::Push(const int m_stack_type, const T &value) {
  if (isFull()) {
    cout << "栈满了" << endl;
    return false;
  }

  if (m_stack_type == STACK1) {
    m_stack_data[++m_stack_1] = value;
    return true;
  } else if (m_stack_type == STACK2) {
    m_stack_data[--m_stack_2] = value;
    return true;
  } else {
    cout << "请指定存放在哪个栈" << endl;
  }
  return false;
}

template <typename T>
void ShareStack<T>::Clear() {
  m_stack_1 = -1;
  m_stack_2 = m_stack_max_size;
}

template <typename T>
bool ShareStack<T>::isFull() {
  if ((m_stack_1 + 1) == m_stack_2) {
    return true;
  }
  return false;
}

int test1() {
  ShareStack<int> sstack;
  bool ret = true;
  int count = 0;
  while (ret) {
    ret = sstack.Push(1, ++count);
    if (ret) cout << "push: " << count << endl;
  }
  sstack.CurrStackSize();
  ret = true;
  while (ret) {
    int p = 0;
    ret = sstack.Pop(1, p);
    if (ret) cout << "pop: " << p << endl;
  }
  cout << "---------------" << endl;
  return 0;
}

int test2() {
  ShareStack<int> *sstack = new ShareStack<int>;
  for (int i = 1; i < 13; ++i) {
    sstack->Push(1, i);
  }
  for (int i = 24; i > 1; --i) {
    sstack->Push(2, i);
  }

  sstack->CurrStackSize();

  bool ret = true;
  int count;
  while (ret) {
    int p = 0;
    ret = sstack->Pop(1, p);
    if (ret) cout << "pop: " << p << endl;
  }
  ret = true;
  while (ret) {
    int p = 0;
    ret = sstack->Pop(2, p);
    if (ret) cout << "pop: " << p << endl;
  }
  cout << "---------------" << endl;
  delete sstack;
  return 0;
}

int main() {
  // ---- test1 -----
  // test1();
  // ----test2 -----
  test2();
}
