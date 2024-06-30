#include <iostream>
#include <string>
using std::cout, std::endl;

#define BLANK_LINE cout << endl;

template <typename T>
struct Node {
  T data;
  Node<T>* prev;
  Node<T>* next;
};

template <typename T>
class DlbList {
 public:
  DlbList();
  ~DlbList();

 public:
  bool ListInsert(int i, const T& value);
  bool ListDel(int i);
  bool GetElem(int i, T& value);
  bool LocatElemFirst(const T& value);

  void ListDisplay();
  void ReverseList();
  int ListLength();

 private:
  Node<T>* m_head;
  int m_length;
};

template <typename T>
DlbList<T>::DlbList() {
  m_head = new Node<T>;
  m_head->prev = nullptr;
  m_head->next = nullptr;
  m_length = 0;
}

template <typename T>
DlbList<T>::~DlbList() {
  Node<T>* currNode = m_head->next;
  Node<T>* tmp;
  while (currNode != nullptr) {
    // tmp = currNode;
    // currNode = currNode->next;
    // delete tmp;
    tmp = currNode;
    currNode = currNode->next;
    delete tmp;
  }
  delete m_head;
  m_length = 0;
  m_head = nullptr;
  cout << "析构~" << endl;
}

template <typename T>
bool DlbList<T>::ListInsert(int i, const T& value) {
  if (i < 1 || i > (m_length + 1)) {
    cout << "插入元素位置不对" << endl;
    return false;
  }

  Node<T>* pCurr = m_head;

  for (int j = 1; j < i; ++j) {
    pCurr = pCurr->next;
  }

  // 栈上分配的局部变量，下一次循环该对象被释放，segment default
  // Node<T> inserNode;
  // inserNode.data = value;
  // inserNode.prev = pCurr;
  // inserNode.next = pCurr->next;

  // if (pCurr->next != nullptr) {
  //   pCurr->next->prev = &inserNode;
  // }
  // pCurr->next = &inserNode;
  Node<T>* inserNode = new Node<T>;
  inserNode->data = value;
  inserNode->prev = pCurr;
  inserNode->next = pCurr->next;

  if (pCurr->next != nullptr) {
    pCurr->next->prev = inserNode;
  }
  pCurr->next = inserNode;

  m_length += 1;
  cout << "插入 " << value << " 到第 " << i << " 个位置" << endl;
  return true;
}

template <typename T>
bool DlbList<T>::ListDel(int i) {
  if (i < 1 || i > m_length) {
    cout << "该位置没有元素要删除" << endl;
    return false;
  }
  Node<T>* pCurr = m_head->next;
  for (int j = 1; j < i; ++j) {
    pCurr = pCurr->next;
  }
  if (pCurr->next != nullptr) {
    pCurr->prev->next = pCurr->next;
    pCurr->next->prev = pCurr->prev;
  } else
    pCurr->prev->next = nullptr;
  cout << "删除第" << i << "个元素" << pCurr->data << endl;

  delete pCurr;
  m_length--;
  return true;
}

template <typename T>
bool DlbList<T>::GetElem(int i, T& value) {
  if (i < 1 || i > m_length) {
    cout << "要查找的位置不正确" << endl;
    return false;
  }
  Node<T>* pCurr = m_head->next;
  for (int j = 1; j < i; ++j) {
    pCurr = pCurr->next;
  }
  value = pCurr->data;
  cout << "第 " << i << " 个位置上的元素为 " << value << endl;
  return true;
}

template <typename T>
bool DlbList<T>::LocatElemFirst(const T& value) {
  Node<T>* pCurr = m_head->next;
  int count = 1;
  while (pCurr != NULL) {
    if (pCurr->data == value) {
      cout << value << " 第一次出现在位置 " << count << endl;
      return true;
    }
    ++count;
    pCurr = pCurr->next;
  }
  cout << "链表中没有值为 " << value << " 的元素" << endl;
  return false;
}

template <typename T>
void DlbList<T>::ListDisplay() {
  Node<T>* currNode = m_head->next;
  cout << "链表中的元素有：";
  while (currNode != nullptr) {
    cout << currNode->data << " ";
    currNode = currNode->next;
  }
}

template <typename T>
void DlbList<T>::ReverseList() {
  if (m_length < 2) {
    cout << "无需翻转链表" << endl;
    return;
  }
  Node<T>* pCurr = m_head->next;
  Node<T>* tmp;
  tmp = pCurr->next;
  pCurr->prev = pCurr->next;
  pCurr->next = nullptr;
  pCurr = tmp;
  while (pCurr != nullptr) {
    tmp = pCurr->next;
    pCurr->next = pCurr->prev;
    if (tmp == nullptr) {
      pCurr->prev = m_head;
      m_head->next = pCurr;
      return;
    }
    pCurr->prev = tmp;
    pCurr = tmp;
  }
}

template <typename T>
int DlbList<T>::ListLength() {
  return m_length;
}

int main() {
  DlbList<std::string> dlbList;
  dlbList.ListInsert(2, "hello1");
  dlbList.ListInsert(1, "hello2");
  dlbList.ListInsert(2, "hello3");
  dlbList.ListInsert(3, "hello4");
  dlbList.ListInsert(1, "hello5");
  dlbList.ListDisplay();

  BLANK_LINE
  dlbList.ListDel(0);
  dlbList.ListDel(4);
  dlbList.ListDel(1);
  dlbList.ListDel(2);
  dlbList.ListInsert(1, "hello5");
  dlbList.ListDisplay();
  BLANK_LINE
  std::string p1, p2;
  dlbList.GetElem(4, p1);
  dlbList.GetElem(2, p1);
  dlbList.GetElem(1, p2);
  cout << "p1 = " << p1 << ", p2 = " << p2 << endl;
  BLANK_LINE

  dlbList.LocatElemFirst("hello");
  dlbList.LocatElemFirst("hello1");
  dlbList.LocatElemFirst("hello2");
  dlbList.LocatElemFirst("hello3");
  dlbList.LocatElemFirst("hello5");
  BLANK_LINE
  dlbList.ListInsert(2, "hello1");
  dlbList.ListInsert(1, "hello2");
  dlbList.ListInsert(2, "hello3");
  dlbList.ListInsert(3, "hello4");
  dlbList.ListInsert(1, "hello5");
  dlbList.ListDisplay();
  BLANK_LINE
  dlbList.ReverseList();
  dlbList.ListDisplay();
  BLANK_LINE
  return 0;
}