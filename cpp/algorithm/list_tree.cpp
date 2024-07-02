#include <cmath>
#include <iostream>

using std::cout, std::endl;
#define BLANK_LINE cout << endl;
#define MAX_SIZE 10

enum NODESIGN { ROOT, CHILD_LEFT, CHILD_RIGHT };

template <typename T>
struct Node {
  T data;
  bool isValid;
};

template <typename T>
class ListBtree {
 public:
  ListBtree();
  ~ListBtree() {}

  int CreateNode(int parentidx, NODESIGN sign, const T& v);
  int getParentIdx(int sonidx);
  int getNodeLevel(int idx);
  int getTreeLevel();
  bool isCompleteBtree();
  void preOrder();
  void preOrder(int idx);
  bool ifValidRangeIdx(int idx) {
    if (idx < 1 || (idx > MAX_SIZE)) {
      cout << "给定的位置不正确" << endl;
      return false;
    }
    return true;
  }
  void Display() {
    for (int i = 1; i <= MAX_SIZE; ++i) {
      if (!treeNode[i].isValid) {
        cout << "第 " << i << " 个位置上没有值" << endl;
        continue;
      }
      cout << "第 " << i << " 个位置上的值为" << treeNode[i].data << endl;
    }
  }

 private:
  Node<T> treeNode[MAX_SIZE + 1];  // 不用索引为0的数组
};

template <typename T>
ListBtree<T>::ListBtree() {
  for (int i = 0; i <= MAX_SIZE; ++i) {
    treeNode[i].isValid = false;
  }
}

template <typename T>
int ListBtree<T>::CreateNode(int parentidx, NODESIGN sign, const T& v) {
  int index = -1;
  if (sign != ROOT) {
    if (!ifValidRangeIdx(parentidx)) {
      return -1;
    }
    if (treeNode[parentidx].isValid == false) {
      cout << "没有父节点" << endl;
      return -1;
    }
  } else {
    index = 1;
  }

  if (sign == CHILD_LEFT) {
    index = parentidx * 2;
  } else if (sign == CHILD_RIGHT) {
    index = parentidx * 2 + 1;
  }
  if (!ifValidRangeIdx(index)) {
    return -1;
  }
  treeNode[index].data = v;
  treeNode[index].isValid = true;
  cout << "创建节点 " << index << " : " << v << endl;
  return index;
}

template <typename T>
int ListBtree<T>::getParentIdx(int sonidx) {
  return 0;
}

template <typename T>
int ListBtree<T>::getNodeLevel(int idx) {
  if (!ifValidRangeIdx(idx)) {
    cout << "给定的位置不正确" << endl;
    return -1;
  }
  if (treeNode[idx].isValid == false) {
    cout << "该位置没有节点" << endl;
    return -1;
  }
  int level = log(idx) / log(2) + 1;
  return level;
}

template <typename T>
int ListBtree<T>::getTreeLevel() {
  int level = 0;
  if (treeNode[1].isValid == false) {
    cout << "空二叉树" << endl;
    return level;
  }
  int max = MAX_SIZE;
  while (max > 0) {
    if (treeNode[max].isValid) {
      level = getNodeLevel(max);
      break;
    }
    --max;
  }
  return level;
}

template <typename T>
bool ListBtree<T>::isCompleteBtree() {
  if (!treeNode[1].isValid) {
    cout << "没有根" << endl;
    return false;
  }
  int i;
  for (i = MAX_SIZE; i > 0; --i) {
    if (treeNode[i].isValid) {
      break;
    }
  }

  for (; i > 0; --i) {
    if (!treeNode[i].isValid) {
      cout << "不是完全二叉树" << endl;
      return false;
    }
  }
  cout << "是完全二叉树" << endl;
  return true;
}

template <typename T>
void ListBtree<T>::preOrder() {
  if (!treeNode[1].isValid) {
    cout << "空二叉树" << endl;
    return;
  }
  preOrder(1);
}

template <typename T>
void ListBtree<T>::preOrder(int idx) {
  if (!treeNode[idx].isValid) {
    return;
  }
  cout << treeNode[idx].data;
  preOrder(idx * 2);
  preOrder(idx * 2 + 1);
}

int main() {
  ListBtree<int> mybtree;
  int level = 0;
  int n = mybtree.CreateNode(1, CHILD_LEFT, 4);
  int n1 = mybtree.CreateNode(n, ROOT, 1);
  int n2 = mybtree.CreateNode(n1, CHILD_LEFT, 2);
  int n3 = mybtree.CreateNode(n1, CHILD_RIGHT, 3);
  int n4 = mybtree.CreateNode(n2, CHILD_LEFT, 4);
  mybtree.CreateNode(n2, CHILD_RIGHT, 5);
  int n5 = mybtree.CreateNode(n3, CHILD_LEFT, 6);
  mybtree.CreateNode(n5, CHILD_RIGHT, 7);
  mybtree.CreateNode(n5, CHILD_LEFT, 8);
  mybtree.CreateNode(n4, CHILD_RIGHT, 9);
  mybtree.Display();

  // level = mybtree.getNodeLevel(n);
  // cout << "node " << n << " level " << level << endl;
  // BLANK_LINE
  level = mybtree.getTreeLevel();
  cout << "tree " << "level " << level << endl;
  BLANK_LINE
  mybtree.isCompleteBtree();

  BLANK_LINE
  mybtree.preOrder();
  BLANK_LINE
  return 0;
}