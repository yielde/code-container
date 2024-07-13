#include "link_stack.h"

template <typename T>
struct AVLNode {
  T data;
  AVLNode *leftchild, *rightchild;
  int balfac;
};

template <typename T>
class AVLTree {
 public:
  AVLTree() { root = nullptr; }
  ~AVLTree() {
    cout << "树析构~" << endl;
    ReleaseNode(root);
  }

  void InsertElem(const T &e) {
    cout << "插入: " << e << " ";
    InsertElem(root, e);
  }
  void InsertElem(AVLNode<T> *&tnode, const T &e);

  void ReleaseNode(AVLNode<T> *tnode) {
    if (tnode != nullptr) {
      ReleaseNode(tnode->leftchild);
      ReleaseNode(tnode->rightchild);
      delete tnode;
    }
  }

  void DisplayIn() {
    cout << "中序：";
    DisplayIn(root);
    cout << endl;
  }
  void DisplayIn(AVLNode<T> *pnode) {
    if (pnode == nullptr) return;
    DisplayIn(pnode->leftchild);
    cout << pnode->data << " ";
    DisplayIn(pnode->rightchild);
  }
  void DisplayPre() {
    cout << "先序：";
    DisplayPre(root);
    cout << endl;
  }
  void DisplayPre(AVLNode<T> *pnode) {
    if (pnode == nullptr) return;
    cout << pnode->data << " ";
    DisplayPre(pnode->leftchild);
    DisplayPre(pnode->rightchild);
  }

 private:
  AVLNode<T> *root;

 private:
  void RotateLeft(AVLNode<T> *&pointer) {
    AVLNode<T> *oldRoot = pointer;  // 最小不平衡树的根节点
    pointer = oldRoot->rightchild;  // 新的根节点为右孩子
    // 如果新的根节点有左子树，把左子树变为旧的根节点的右子树
    oldRoot->rightchild = pointer->leftchild;
    // 把旧根节点变为新根节点的左子树
    pointer->leftchild = oldRoot;
    // 调整后新根和旧根的因子都为0
    pointer->balfac = 0;
    oldRoot->balfac = 0;
  }

  void RotateRight(AVLNode<T> *&pointer) {
    AVLNode<T> *oldRoot = pointer;
    pointer = oldRoot->leftchild;
    oldRoot->leftchild = pointer->rightchild;
    pointer->rightchild = oldRoot;
    pointer->balfac = 0;
    oldRoot->balfac = 0;
  }

  void RotateLeftRight(AVLNode<T> *&pointer) {
    AVLNode<T> *A = pointer;
    AVLNode<T> *B = A->leftchild;
    AVLNode<T> *C = B->rightchild;

    B->rightchild = C->leftchild;
    C->leftchild = B;

    if (C->balfac >= 0)  // C带的是左孩子
      B->balfac = 0;  // C的左孩子变为了B的右孩子，B的平衡因子变为0
    else              // C带的是右孩子
      B->balfac = 1;  // B只有左孩子，B的平衡因子为1

    A->leftchild = C->rightchild;  // 将C的右孩子变为A的左孩子
    C->rightchild = A;

    if (C->balfac == 1)
      A->balfac = -1;
    else
      A->balfac = 0;

    C->balfac = 0;
    pointer = C;
  }

  void RotateRightLeft(AVLNode<T> *&pointer) {
    AVLNode<T> *A = pointer;
    AVLNode<T> *B = A->rightchild;
    AVLNode<T> *C = B->leftchild;

    B->leftchild = C->rightchild;
    C->rightchild = B;
    if (C->balfac <= 0)
      B->balfac = 0;
    else
      B->balfac = -1;

    A->rightchild = C->leftchild;
    C->leftchild = A;

    if (C->balfac == -1)
      A->balfac = 1;
    else
      A->balfac = 0;

    C->balfac = 0;
    pointer = C;
  }
};

template <typename T>
void AVLTree<T>::InsertElem(AVLNode<T> *&tnode, const T &e) {
  AVLNode<T> *point = tnode;
  AVLNode<T> *parent = nullptr;

  LinkStack<AVLNode<T> *> track;

  while (point != nullptr) {
    if (e == point->data) return;

    parent = point;
    track.Push(parent);

    if (e > point->data)
      point = point->rightchild;
    else
      point = point->leftchild;
  }

  point = new AVLNode<T>;
  point->data = e;
  point->leftchild = nullptr;
  point->rightchild = nullptr;
  point->balfac = 0;

  if (parent == nullptr) {
    tnode = point;
    return;
  }
  if (e > parent->data)
    parent->rightchild = point;
  else
    parent->leftchild = point;

  while (!track.IsEmpty()) {
    if (track.Pop(parent)) {
      if (parent->leftchild == point)
        parent->balfac++;
      else
        parent->balfac--;
    }
    if (parent->balfac < -1 || parent->balfac > 1) {
      if (parent->balfac > 0 && point->balfac > 0) {
        RotateRight(parent);
      } else if (parent->balfac < 0 && point->balfac < 0) {
        RotateLeft(parent);
      } else if (parent->balfac > 0 && point->balfac < 0) {
        RotateLeftRight(parent);
      } else {
        RotateRightLeft(parent);
      }
      if (track.IsEmpty()) {
        root = parent;
      } else {
        AVLNode<T> *pParentPoint = nullptr;
        track.GetTop(pParentPoint);
        if (pParentPoint->data > parent->data)
          pParentPoint->leftchild = parent;
        else
          pParentPoint->rightchild = parent;
      }
      cout << "调整：" << point->data << " " << point->balfac
           << " parent: " << parent->data << " " << parent->balfac << endl;
      break;
    } else if (parent->balfac == 0) {
      cout << "平衡，不需要调整" << endl;
      break;
    } else {
      point = parent;
    }
  }
}

int main() {
  AVLTree<int> mybtr;
  // int array[] = {60, 40, 70, 120, 95};
  int array[] = {12, 4, 1, 3, 7, 8, 10, 9, 2, 11, 6, 5};

  int acount = sizeof(array) / sizeof(int);
  for (int i = 0; i < acount; ++i) mybtr.InsertElem(array[i]);
  mybtr.DisplayIn();
  mybtr.DisplayPre();
  return 0;
}