#include <cstring>
#include <iostream>
#include <iterator>
using std::cout, std::endl, std::fill_n;

#define BLACK_LINE cout << endl;
#define STRIP_LINE                                    \
  cout << endl;                                       \
  fill_n(std::ostream_iterator<char>(cout), 50, '-'); \
  cout << endl;

template <typename T>
struct QueueNode {
  T data;
  QueueNode<T> *next;
};

template <typename T>
class Queue {
 public:
  Queue() {
    m_front = new QueueNode<T>;
    m_front->next = nullptr;
    m_rear = m_front;
    m_length = 0;
  }
  ~Queue() {
    QueueNode<T> *currNode = m_rear->next;
    QueueNode<T> *tmp;
    while (currNode != nullptr) {
      tmp = currNode;
      currNode = currNode->next;
      delete tmp;
    }
    delete m_rear;
    m_length = 0;
    m_front = m_rear = 0;
    cout << "队列析构~" << endl;
  }

 public:
  bool EnQueue(const T &v) {
    QueueNode<T> *newNode = new QueueNode<T>;
    newNode->data = v;
    newNode->next = nullptr;
    m_front->next = newNode;
    m_front = newNode;
    // cout << v << " 入队" << endl;
    m_length++;
    return true;
  }
  bool DeQueue(T &v) {
    if (IsEmpty()) {
      cout << "空队列" << endl;
      return false;
    }
    QueueNode<T> *denode = m_rear->next;
    v = denode->data;
    m_rear->next = denode->next;
    m_length--;
    if (m_front == denode) m_front = m_rear;
    delete denode;
    // cout << v << " 出队" << endl;
    return true;
  }
  bool GetHead(T &v) {
    if (IsEmpty()) {
      cout << "空队列" << endl;
      return false;
    }
    v = m_front->data;
    return true;
  }
  void DisplayQueue() {
    if (IsEmpty()) {
      cout << "队列为空" << endl;
      return;
    }
    cout << "队列中元素有: " << endl;
    QueueNode<T> *rear = m_rear->next;
    while (rear != nullptr) {
      cout << rear->data << endl;
      rear = rear->next;
    }
  }
  bool IsEmpty() { return m_length == 0; }
  int Length() { return m_length; }

 private:
  QueueNode<T> *m_front;
  QueueNode<T> *m_rear;
  int m_length;
};

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

enum CHILDSIGN { ROOT, LEFT, RIGHT };

template <typename T>
struct BtreeNode {
  T data;
  BtreeNode<T> *leftChild;
  BtreeNode<T> *rightChild;
};

template <typename T>
struct BtreeNodeExt {
  BtreeNode<T> *point;
  CHILDSIGN sign;
};

template <typename T>
class ListBTree {
 public:
  ListBTree();
  ~ListBTree();

  BtreeNode<T> *CreateNode(BtreeNode<T> *parentNode, CHILDSIGN sign,
                           const T &v);
  void CreateBTreeAccordPI(const char *pP_T, const char *pI_T) {
    CreateBTreeAccordPI(root, pP_T, pI_T, strlen(pP_T));
  }
  void CreateBTreeAccordPI(BtreeNode<T> *&tnode, const char *pP_T,
                           const char *pI_T, int n);
  void ReleaseNode(BtreeNode<T> *node);
  void CreateBtreeAcorrdPT(const char *pstr);
  void CreateBtreeAcorrdPTRecu(BtreeNode<T> *&tnode, const char *&pstr);

  void BreadthOrder() {
    cout << "广度优先遍历" << endl;
    BreadthOrder(root);
  }  // 广度优先遍历，借助队列
  void BreadthOrder(BtreeNode<T> *tnode);  // 广度优先遍历，借助队列

 public:
  // 深度优先的3种遍历
  void preOrder() {
    cout << "前序遍历:" << endl;
    preOrder(root);
    STRIP_LINE
  }
  void preOrder(BtreeNode<T> *tnode);

  void inOrder() {
    cout << "中序遍历:" << endl;
    inOrder(root);
    STRIP_LINE
  }

  void inOrder(BtreeNode<T> *tnode);

  void postOrder() {
    cout << "后序遍历:" << endl;
    postOrder(root);
    STRIP_LINE
  }
  void postOrder(BtreeNode<T> *tnode);

  void preOrderStack() {
    cout << "(栈方式)前序遍历: " << endl;
    preOrderStack(root);
    STRIP_LINE
  }
  void preOrderStack(BtreeNode<T> *tnode);

  void inOrderStack() {
    cout << "(栈方式)中序遍历: " << endl;
    inOrderStack(root);
    STRIP_LINE
  }
  void inOrderStack(BtreeNode<T> *tnode);

  void postOrderStack() {
    cout << "(栈方式)后序遍历: " << endl;
    postOrderStack(root);
    STRIP_LINE
  }
  void postOrderStack(BtreeNode<T> *tnode);
  int getSize() { return getSize(root); }
  int getSize(BtreeNode<T> *tnode);

  int getHeight() { return getHeight(root); }
  int getHeight(BtreeNode<T> *tnode);

  BtreeNode<T> *SearchElem(const T &v) { return SearchElem(v, root); }
  BtreeNode<T> *SearchElem(const T &v, BtreeNode<T> *tnode);

 private:
  BtreeNode<T> *root;
};

template <typename T>
ListBTree<T>::ListBTree() {
  root = nullptr;
}

template <typename T>
ListBTree<T>::~ListBTree() {
  ReleaseNode(root);
  cout << "B树析构~" << endl;
}

template <typename T>
BtreeNode<T> *ListBTree<T>::CreateNode(BtreeNode<T> *parentNode, CHILDSIGN sign,
                                       const T &v) {
  BtreeNode<T> *newNode = new BtreeNode<T>;
  newNode->data = v;
  newNode->leftChild = nullptr;
  newNode->rightChild = nullptr;

  if (sign == ROOT)
    root = newNode;
  else if (sign == LEFT)
    parentNode->leftChild = newNode;
  else if (sign == RIGHT)
    parentNode->rightChild = newNode;
  cout << "创建node " << newNode->data << endl;
  return newNode;
}

template <typename T>
void ListBTree<T>::CreateBTreeAccordPI(BtreeNode<T> *&tnode, const char *pP_T,
                                       const char *pI_T, int n) {
  if (n == 0) {
    tnode = nullptr;
  } else {
    int index = 0;
    while (pP_T[0] != pI_T[index]) {
      index++;
    }
    tnode = new BtreeNode<T>;
    tnode->data = pI_T[index];
    cout << "insert " << tnode->data << endl;

    CreateBTreeAccordPI(tnode->leftChild, pP_T + 1, pI_T, index);

    CreateBTreeAccordPI(tnode->rightChild, pP_T + 1 + index, pI_T + 1 + index,
                        n - index - 1);
  }
}

template <typename T>
void ListBTree<T>::ReleaseNode(BtreeNode<T> *node) {
  if (node != nullptr) {
    ReleaseNode(node->leftChild);
    ReleaseNode(node->rightChild);
  }
  delete node;
}

template <typename T>
void ListBTree<T>::CreateBtreeAcorrdPT(const char *pstr) {
  CreateBtreeAcorrdPTRecu(root, pstr);
}

template <typename T>
void ListBTree<T>::CreateBtreeAcorrdPTRecu(BtreeNode<T> *&tnode,
                                           const char *&pstr) {
  if (*pstr == '#')
    tnode = nullptr;
  else {
    tnode = new BtreeNode<T>;
    tnode->data = *pstr;
    cout << "扩展二叉树前序遍历插入 " << tnode->data << endl;
    CreateBtreeAcorrdPTRecu(tnode->leftChild, ++pstr);
    CreateBtreeAcorrdPTRecu(tnode->rightChild, ++pstr);
  }
}

template <typename T>
void ListBTree<T>::BreadthOrder(BtreeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }
  Queue<BtreeNode<T> *> queue;
  queue.EnQueue(tnode);
  BtreeNode<T> *v;
  while (!queue.IsEmpty()) {
    queue.DeQueue(v);
    cout << v->data;
    if (v->leftChild != nullptr) {
      queue.EnQueue(v->leftChild);
    }
    if (v->rightChild != nullptr) {
      queue.EnQueue(v->rightChild);
    }
  }
  BLACK_LINE
}

template <typename T>
void ListBTree<T>::preOrder(BtreeNode<T> *tnode) {
  if (tnode != nullptr) {
    cout << tnode->data;
    preOrder(tnode->leftChild);
    preOrder(tnode->rightChild);
  }
}

template <typename T>
void ListBTree<T>::inOrder(BtreeNode<T> *tnode) {
  if (tnode != nullptr) {
    inOrder(tnode->leftChild);
    cout << tnode->data;
    inOrder(tnode->rightChild);
  }
}

template <typename T>
void ListBTree<T>::postOrder(BtreeNode<T> *tnode) {
  if (tnode != nullptr) {
    postOrder(tnode->leftChild);
    postOrder(tnode->rightChild);
    cout << tnode->data;
  }
}
template <typename T>
void ListBTree<T>::preOrderStack(BtreeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }
  LinkStack<BtreeNode<T> *> stack;
  stack.Push(tnode);
  BtreeNode<T> *v;
  while (!stack.IsEmpty()) {
    stack.Pop(v);
    cout << v->data;

    if (v->rightChild != nullptr) {
      stack.Push(v->rightChild);
    }
    if (v->leftChild != nullptr) {
      stack.Push(v->leftChild);
    }
  }
  BLACK_LINE
}

template <typename T>
void ListBTree<T>::inOrderStack(BtreeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }
  LinkStack<BtreeNode<T> *> stack;
  BtreeNode<T> *v = tnode;
  BtreeNode<T> *p = nullptr;

  stack.Push(v);
  while (!stack.IsEmpty()) {
    while (v->leftChild != nullptr) {
      stack.Push(v->leftChild);
      v = v->leftChild;
    }
    stack.Pop(p);
    cout << p->data;
    if (p->rightChild != nullptr) {
      stack.Push(p->rightChild);
      v = p->rightChild;
    }
  }
  BLACK_LINE
}

template <typename T>
void ListBTree<T>::postOrderStack(BtreeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }

  LinkStack<BtreeNodeExt<T>> stack;
  BtreeNodeExt<T> tmp;
  BtreeNode<T> *proot = tnode;

  do {
    if (root == nullptr) {
      cout << "空树" << endl;
      return;
    }

    while (proot != nullptr) {
      tmp.sign = LEFT;
      tmp.point = proot;
      stack.Push(tmp);
      proot = proot->leftChild;
    }

    while (!stack.IsEmpty()) {
      stack.Pop(tmp);
      if (tmp.sign == LEFT) {
        tmp.sign = RIGHT;
        stack.Push(tmp);
        proot = tmp.point->rightChild;
        break;
      } else {
        cout << tmp.point->data;
      }
    }
  } while (!stack.IsEmpty());
  BLACK_LINE
}

template <typename T>
int ListBTree<T>::getSize(BtreeNode<T> *tnode) {
  if (tnode == nullptr) return 0;
  return getSize(tnode->leftChild) + getSize(tnode->rightChild) + 1;
}

template <typename T>
int ListBTree<T>::getHeight(BtreeNode<T> *tnode) {
  if (tnode == nullptr) return 0;
  int lchild = getHeight(tnode->leftChild);
  int rchild = getHeight(tnode->rightChild);
  if (lchild > rchild) return lchild + 1;
  return rchild + 1;
}

template <typename T>
BtreeNode<T> *ListBTree<T>::SearchElem(const T &v, BtreeNode<T> *tnode) {
  if (tnode == nullptr) return nullptr;
  if (tnode->data == v) return tnode;

  BtreeNode<T> *lchild = SearchElem(v, tnode->leftChild);
  if (lchild != nullptr) return lchild;
  BtreeNode<T> *rchild = SearchElem(v, tnode->rightChild);
  return rchild;
}

int test_LinkStack() {
  LinkStack<int> lstack;
  lstack.Push(1);
  lstack.Push(2);
  lstack.Push(3);
  lstack.DispList();
  cout << "stack size: " << lstack.StackSize() << endl;
  BLACK_LINE
  int p;
  lstack.GetTop(p);
  cout << "top " << p << endl;
  lstack.GetTop(p);
  cout << "top " << p << endl;
  BLACK_LINE
  int n;
  lstack.Pop(n);
  cout << "pop " << n << endl;
  lstack.Pop(n);
  cout << "pop " << n << endl;
  lstack.Pop(n);
  cout << "pop " << n << endl;
  lstack.DispList();
  cout << "stack size：" << lstack.StackSize() << endl;
  BLACK_LINE
  return 0;
}

int test_btree() {
  ListBTree<char> lbtree;
  // BtreeNode<int> *n_root = lbtree.CreateNode(nullptr, ROOT, 1);
  // BtreeNode<int> *n_root_l = lbtree.CreateNode(n_root, LEFT, 2);
  // BtreeNode<int> *n_root_r = lbtree.CreateNode(n_root, RIGHT, 3);
  // lbtree.CreateNode(n_root_r, RIGHT, 4);
  // BtreeNode<int> *n_root_l_l = lbtree.CreateNode(n_root_l, LEFT, 5);
  // BtreeNode<int> *n_root_l_r = lbtree.CreateNode(n_root_l, RIGHT, 6);
  // lbtree.CreateNode(n_root_l_l, LEFT, 7);
  // BLACK_LINE

  // 前序遍历扩展二叉树生成唯一二叉树
  // lbtree.CreateBtreeAcorrdPT("ABD###C#E##");

  // 前序遍历 和 中序遍历序列生成二叉树

  lbtree.CreateBTreeAccordPI("ABDCE", "DBACE");
  // 测试深度优先的三种遍历（递归）
  lbtree.preOrder();
  lbtree.inOrder();
  lbtree.postOrder();
  // 深度优先三种遍历（非递归）
  lbtree.preOrderStack();
  lbtree.inOrderStack();
  lbtree.postOrderStack();

  // 广度优先遍历
  lbtree.BreadthOrder();
  STRIP_LINE
  // 获取树节点数
  cout << "节点总数: " << lbtree.getSize() << endl;
  STRIP_LINE
  cout << "树的高度: " << lbtree.getHeight() << endl;
  STRIP_LINE
  BtreeNode<char> *ret = lbtree.SearchElem('D');
  if (ret) {
    cout << "D所在树的节点地址为 " << ret << " 值 " << ret->data << endl;
  } else {
    cout << "D不在树中" << endl;
  }
  STRIP_LINE
  return 0;
}

int test_queue() {
  int q;
  Queue<int> queue;
  queue.GetHead(q);
  queue.EnQueue(1);
  queue.EnQueue(2);
  queue.EnQueue(3);
  queue.DisplayQueue();
  STRIP_LINE
  queue.GetHead(q);
  cout << "队首元素为 " << q << endl;
  STRIP_LINE
  queue.DisplayQueue();
  STRIP_LINE
  queue.DeQueue(q);
  queue.DeQueue(q);
  queue.DeQueue(q);
  queue.DeQueue(q);
  BLACK_LINE
  return 0;
}

int main() {
  // test_queue();
  // test_LinkStack();
  test_btree();
  return 0;
}
