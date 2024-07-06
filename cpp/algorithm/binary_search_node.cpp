#include <vector>

#include "publiclib.h"
template <typename T>
struct treeNode {
  T data;
  treeNode *leftchild, *rightchild;
};

template <typename T>
class BinarySearchTree {
 public:
  BinarySearchTree() { root = nullptr; }
  ~BinarySearchTree() {
    cout << "析构~" << endl;
    ReleaseNode(root);
  }

  void inOrder() {
    cout << "中序遍历: ";
    inOrder(root);
  }
  void inOrder(treeNode<T> *tnode);

  void preOrder() {
    cout << "先序遍历: ";
    preOrder(root);
  }
  void preOrder(treeNode<T> *tnode);
  void InsertElem(const T &v) { InsertElem(root, v); }
  void InsertElem(treeNode<T> *&tnode, const T &v);

  void DeleteElem(const T &v) { DeleteElem(root, v); }
  void DeleteElem(treeNode<T> *&tnode, const T &v);

  treeNode<T> *SearchElem(const T &v) {
    cout << "递归查找" << endl;
    return SearchElem(root, v);
  }
  treeNode<T> *SearchElem(treeNode<T> *tnode, const T &v);
  treeNode<T> *SearchElemNoRecu(const T &v) {
    cout << "非递归查找" << endl;
    return SearchElemNoRecu(root, v);
  }
  treeNode<T> *SearchElemNoRecu(treeNode<T> *tnode, const T &v);

  void getRoot() {
    if (root != nullptr) {
      cout << "root: " << root->data << " " << root << endl;
    }
  }

  treeNode<T> *SearchMaxValueNode() {
    if (root == nullptr) {
      cout << "空树" << endl;
      return nullptr;
    }
    treeNode<T> *currNode = root;
    while (currNode->rightchild != nullptr) {
      currNode = currNode->rightchild;
    }
    return currNode;
  }

  treeNode<T> *SearchMinValueNode() {
    if (root == nullptr) {
      cout << "空树" << endl;
      return nullptr;
    }
    treeNode<T> *currNode = root;
    while (currNode->leftchild != nullptr) {
      currNode = currNode->leftchild;
    }
    return currNode;
  }

  treeNode<T> *GetPriorNodeByInOrder(const T &v) {
    // 应当提前确认传入的节点是否有效，用传入的值代替只为了验证思路
    if (root == nullptr) {
      cout << "空树" << endl;
      return nullptr;
    }

    treeNode<T> *currNodePrior = nullptr;
    treeNode<T> *currNode = root;

    while (currNode != nullptr) {
      if (v > currNode->data) {
        currNodePrior = currNode;
        currNode = currNode->rightchild;

      } else if (v == currNode->data) {
        currNode = currNode->leftchild;
      } else {
        currNode = currNode->leftchild;
      }
    }
    return currNodePrior;
  }

  treeNode<T> *GetNextNodeByInOrder(const T &v) {
    if (root == nullptr) {
      cout << "空树" << endl;
      return nullptr;
    }
    treeNode<T> *currNodeNext = nullptr;
    treeNode<T> *currNode = root;

    while (currNode != nullptr) {
      if (v > currNode->data) {
        currNode = currNode->rightchild;
      } else if (v == currNode->data) {
        currNode = currNode->rightchild;
      } else {
        currNodeNext = currNode;
        currNode = currNode->leftchild;
      }
    }
    return currNodeNext;
  }

 private:
  treeNode<T> *root;

 private:
  void ReleaseNode(treeNode<T> *tnode) {
    if (tnode != nullptr) {
      ReleaseNode(tnode->leftchild);
      ReleaseNode(tnode->rightchild);
      delete tnode;
    }
  }
};

template <typename T>
void BinarySearchTree<T>::inOrder(treeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }
  if (tnode != nullptr) {
    inOrder(tnode->leftchild);
    cout << tnode->data << " ";
    inOrder(tnode->rightchild);
  }
}

template <typename T>
void BinarySearchTree<T>::preOrder(treeNode<T> *tnode) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }
  if (tnode != nullptr) {
    cout << tnode->data << " ";
    preOrder(tnode->leftchild);
    preOrder(tnode->rightchild);
  }
}

template <typename T>
void BinarySearchTree<T>::InsertElem(treeNode<T> *&tnode, const T &v) {
  if (tnode == nullptr) {
    tnode = new treeNode<T>;
    tnode->data = v;
    tnode->leftchild = nullptr;
    tnode->rightchild = nullptr;
    return;
  }
  if (v < tnode->data) {
    InsertElem(tnode->leftchild, v);
  } else if (v > tnode->data) {
    InsertElem(tnode->rightchild, v);
  } else {
    cout << "已经存在该节点" << endl;
  }
}

template <typename T>
void BinarySearchTree<T>::DeleteElem(treeNode<T> *&tnode, const T &v) {
  if (root == nullptr) {
    cout << "空树" << endl;
    return;
  }

  if (v > tnode->data) {
    DeleteElem(tnode->rightchild, v);
  } else if (v < tnode->data) {
    DeleteElem(tnode->leftchild, v);
  } else if (v == tnode->data) {
    // 没有子节点
    treeNode<T> *delNode = tnode;
    if (tnode->leftchild == nullptr && tnode->rightchild == nullptr) {
      tnode = nullptr;
    } else if (tnode->leftchild == nullptr) {
      tnode = tnode->rightchild;
    } else if (tnode->rightchild == nullptr) {
      tnode = tnode->leftchild;
    } else {
      treeNode<T> *currNodePare = tnode;
      delNode = tnode->leftchild;
      while (delNode->rightchild != nullptr) {
        currNodePare = delNode;
        delNode = delNode->rightchild;
      }
      tnode->data = delNode->data;
      if (tnode == currNodePare) {
        tnode->leftchild = delNode->leftchild;
      } else {
        currNodePare->rightchild = delNode->leftchild;
      }
    }
    delete delNode;
  } else {
    cout << "节点 " << v << " 不在树种" << endl;
  }
}

template <typename T>
treeNode<T> *BinarySearchTree<T>::SearchElem(treeNode<T> *tnode, const T &v) {
  if (tnode == nullptr) return nullptr;
  if (tnode->data == v) return tnode;
  if (v > tnode->data)
    return SearchElem(tnode->rightchild, v);
  else
    return SearchElem(tnode->leftchild, v);
}

template <typename T>
treeNode<T> *BinarySearchTree<T>::SearchElemNoRecu(treeNode<T> *tnode,
                                                   const T &v) {
  while (tnode != nullptr) {
    if (tnode->data == v) return tnode;
    if (v > tnode->data)
      tnode = tnode->rightchild;
    else
      tnode = tnode->leftchild;
  }
  return nullptr;
}

int main() {
  BinarySearchTree<int> btree;
  std::vector<int> vec = {23, 17, 11, 19, 8, 12, 34, 66, 21, 22, 65, 44};
  size_t len = vec.size();
  for (size_t i = 0; i < len; ++i) {
    btree.InsertElem(vec[i]);
  }
  btree.getRoot();
  btree.inOrder();
  btree.preOrder();
  STRIP_LINE
  treeNode<int> *p;
  // int pp = 5;
  // cout << "查找节点 " << pp << " ";
  // p = btree.SearchElem(pp);
  // if (p)
  //   cout << p->data << endl;
  // else
  //   cout << "没有该节点" << endl;
  // pp = 23;
  // cout << "查找节点 " << pp << " ";
  // p = btree.SearchElem(pp);
  // if (p)
  //   cout << p->data << endl;
  // else
  //   cout << "没有该节点" << endl;
  // pp = 19;
  // cout << "查找节点 " << pp << " ";
  // p = btree.SearchElem(pp);
  // if (p)
  //   cout << p->data << endl;
  // else
  //   cout << "没有该节点" << endl;

  STRIP_LINE
  // 非递归查找
  int pp = 5;
  cout << "查找节点 " << pp << " ";
  p = btree.SearchElemNoRecu(pp);
  if (p)
    cout << p->data << endl;
  else
    cout << "没有该节点" << endl;
  pp = 23;
  cout << "查找节点 " << pp << " ";
  p = btree.SearchElemNoRecu(pp);
  if (p)
    cout << p->data << endl;
  else
    cout << "没有该节点" << endl;
  pp = 19;
  cout << "查找节点 " << pp << " ";
  p = btree.SearchElemNoRecu(pp);
  if (p)
    cout << p->data << endl;
  else
    cout << "没有该节点" << endl;
  STRIP_LINE
  btree.DeleteElem(19);
  btree.DeleteElem(23);

  btree.inOrder();
  btree.preOrder();

  btree.getRoot();
  STRIP_LINE
  p = btree.SearchMaxValueNode();
  if (p != nullptr) cout << "最大节点 " << p->data << endl;
  STRIP_LINE

  p = btree.SearchMinValueNode();
  if (p != nullptr) cout << "最小节点 " << p->data << endl;
  STRIP_LINE

  p = btree.GetPriorNodeByInOrder(22);
  if (p != nullptr) {
    cout << "前驱节点为 " << p->data << endl;
  }
  STRIP_LINE

  p = btree.GetNextNodeByInOrder(17);
  if (p != nullptr) {
    cout << "后继节点为 " << p->data << endl;
  }
  STRIP_LINE
  return 0;
}