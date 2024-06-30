#include <iostream>
using std::cout, std::endl;

#define BLAK_LINE cout << endl;

template <typename T>
struct Node {
  T data;         // 存放数据
  Node<T>* next;  // 指向下一个节点
};

template <typename T>
class LinkList {
 public:
  LinkList();
  ~LinkList();

 public:
  bool ListInsert(int i, const T& value);
  bool ListDelete(int i);
  bool GetElem(int i, T& value);
  bool LocateElemFirst(const T& value);

  void DisplayList();
  void ReverseList();
  int ListLength();
  bool isEmpty();

 private:
  Node<T>* m_head;
  int m_length;
};

template <typename T>
LinkList<T>::LinkList() {
  m_length = 0;
  m_head = new Node<T>;  // 构造一个头节点，不计入链表长度
  m_head->next = nullptr;
}

template <typename T>
LinkList<T>::~LinkList() {
  Node<T>* currentNode = m_head->next;
  Node<T>* tmp;
  while (currentNode != nullptr) {
    tmp = currentNode;
    currentNode = currentNode->next;
    delete tmp;
  }
  delete m_head;
  m_length = 0;
  cout << "析构链表" << endl;
}

template <typename T>
bool LinkList<T>::ListInsert(int i, const T& value) {
  if (i < 1 || i > (m_length + 1)) {
    cout << "插入元素的位置不正确" << endl;
    return false;
  }

  Node<T>* currentNode = m_head;

  for (int j = 1; j < i; ++j) {
    currentNode = currentNode->next;
  }
  Node<T>* newNode = new Node<T>;
  newNode->data = value;
  newNode->next = currentNode->next;
  currentNode->next = newNode;

  m_length += 1;
  cout << "插入第 " << i << " 个元素 " << value << endl;
  return true;
}

template <typename T>
bool LinkList<T>::ListDelete(int i) {
  if (i < 1 || i > m_length) {
    cout << "无法删除，不存在该元素" << endl;
    return false;
  }
  Node<T>* currentNode = m_head;
  for (int j = 1; j < i; ++j) {
    currentNode = currentNode->next;
  }
  Node<T>* DelNode = currentNode->next;
  currentNode->next = currentNode->next->next;
  m_length -= 1;
  cout << "删除第 " << i << " 个元素 " << DelNode->data << endl;
  delete DelNode;
  return true;
}

template <typename T>
bool LinkList<T>::GetElem(int i, T& value) {
  if (i < 1 || i > m_length) {
    cout << "要找的元素的位置不正确" << endl;
    return false;
  }
  Node<T>* currentNode = m_head;

  for (int j = 1; j < i; ++j) {
    currentNode = currentNode->next;
  }
  value = currentNode->next->data;
  return true;
}

template <typename T>
bool LinkList<T>::LocateElemFirst(const T& value) {
  Node<T>* currentNode = m_head->next;
  int i = 1;
  while (currentNode != nullptr) {
    if (currentNode->data == value) {
      cout << value << " 在第 " << i << " 个位置" << endl;
      return true;
    }
    currentNode = currentNode->next;
    ++i;
  }
  cout << "链表中不存在该元素" << endl;
  return true;
}

template <typename T>
void LinkList<T>::DisplayList() {
  Node<T>* currentNode = m_head->next;
  cout << "当前列表中的Node有: ";

  while (currentNode != NULL) {
    cout << currentNode->data;
    currentNode = currentNode->next;
  }

  cout << endl;
}

template <typename T>
void LinkList<T>::ReverseList() {
  if (m_length <= 1) {
    return;
  }
  Node<T>* nnNode = m_head->next->next;
  Node<T>* tmp;
  m_head->next->next = nullptr;

  while (nnNode != nullptr) {
    tmp = nnNode;
    nnNode = nnNode->next;
    tmp->next = m_head->next;
    m_head->next = tmp;
  }
}

int main() {
  LinkList<int> linklist;
  linklist.ListInsert(2, 1);
  linklist.ListInsert(1, 1);
  linklist.ListInsert(2, 2);
  linklist.ListInsert(3, 3);
  linklist.ListInsert(2, 4);
  linklist.DisplayList();
  BLAK_LINE
  linklist.ListDelete(9);
  linklist.ListDelete(3);
  linklist.DisplayList();
  BLAK_LINE
  int i = 10, v;
  bool ret = linklist.GetElem(i, v);
  if (ret) {
    cout << "找到第 " << i << "个元素 " << v << endl;
  }
  i = 2;
  ret = linklist.GetElem(i, v);
  if (ret) {
    cout << "找到第 " << i << "个元素 " << v << endl;
  }
  linklist.DisplayList();
  BLAK_LINE

  linklist.LocateElemFirst(5);
  linklist.LocateElemFirst(2);
  linklist.LocateElemFirst(4);
  BLAK_LINE

  linklist.ReverseList();
  linklist.DisplayList();
  return 0;
}