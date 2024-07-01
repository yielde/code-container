#include <iostream>

using std::cout, std::endl;
#define BLANK_LINE cout << endl;

template <typename T>
struct Node {
  T data;
  Node<T>* next;
};

template <typename T>
class LinkQueue {
 public:
  LinkQueue();
  ~LinkQueue();

  bool EnQueue(const T& v);
  bool DeQueue(T& v);
  void DisplayQueue();
  bool IsEmpty();
  int Length();

 private:
  int m_length;
  Node<T>* m_head;  // head不存放数据，指向第一个（即队尾）元素
  Node<T>* m_tail;
};

template <typename T>
LinkQueue<T>::LinkQueue() {
  m_head = new Node<T>;
  m_head->next = nullptr;
  m_tail = m_head;
  m_length = 0;
}

template <typename T>
LinkQueue<T>::~LinkQueue() {
  Node<T>* currNode = m_head->next;
  Node<T>* tmp;
  while (currNode != nullptr) {
    tmp = currNode;
    currNode = currNode->next;
    delete tmp;
  }
  delete m_head;
  m_length = 0;
  m_head = m_tail = nullptr;
}

template <typename T>
bool LinkQueue<T>::EnQueue(const T& v) {
  Node<T>* inserNode = new Node<T>;
  inserNode->next = nullptr;
  inserNode->data = v;

  m_tail->next = inserNode;
  m_tail = inserNode;
  ++m_length;
  cout << inserNode->data << " 入队" << endl;
  return true;
}

template <typename T>
bool LinkQueue<T>::DeQueue(T& v) {
  if (IsEmpty()) {
    cout << "队列为空" << endl;
    return false;
  }
  Node<T>* DeNode = m_head->next;
  if (DeNode->next == nullptr) m_head = m_tail;
  m_head->next = DeNode->next;
  v = DeNode->data;
  delete DeNode;
  --m_length;
  cout << v << " 出队" << endl;
  return true;
}

template <typename T>
void LinkQueue<T>::DisplayQueue() {
  Node<T>* currNode = m_head->next;
  cout << "队列中的元素有：";
  while (currNode != nullptr) {
    cout << currNode->data << " ";
    currNode = currNode->next;
  }
  cout << endl;
}

template <typename T>
bool LinkQueue<T>::IsEmpty() {
  if (m_head == m_tail) return true;
  return false;
}

template <typename T>
int LinkQueue<T>::Length() {
  return m_length;
}

int main() {
  LinkQueue<int> lqueue;
  for (int i = 0; i < 10; ++i) {
    lqueue.EnQueue(i);
  }
  lqueue.DisplayQueue();
  BLANK_LINE

  int p;
  lqueue.DeQueue(p);
  lqueue.DeQueue(p);
  lqueue.DeQueue(p);
  lqueue.DeQueue(p);
  lqueue.DisplayQueue();
  int len = lqueue.Length();
  cout << len << endl;
  BLANK_LINE

  return 0;
}