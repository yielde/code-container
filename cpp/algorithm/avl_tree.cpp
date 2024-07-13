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
  void DelElem(const T &e) { DelElem(root, e); }
  void DelElem(AVLNode<T> *&tNode, const T &e);

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

template <typename T>
void AVLTree<T>::DelElem(AVLNode<T> *&tNode, const T &e) {
  AVLNode<T> *delNode = tNode;
  AVLNode<T> *parent = nullptr;

  LinkStack<AVLNode<T> *> track;

  while (delNode != nullptr) {
    if (delNode->data == e) break;

    parent = delNode;
    track.Push(parent);

    if (delNode->data > e)
      delNode = delNode->leftchild;
    else
      delNode = delNode->rightchild;
  }

  if (delNode == nullptr) {
    cout << "没有要删除的节点" << endl;
    return;
  }
  // 删除的是根节点
  if (parent == nullptr) {
    if (delNode->leftchild == nullptr && delNode->rightchild == nullptr)
      tNode = nullptr;
    else if (delNode->leftchild == nullptr) {
      tNode = delNode->rightchild;
    } else {
      tNode = delNode->leftchild;
    }
  }

  // 删除非根节点
  AVLNode<T> *tmp = nullptr;
  // 删除叶子节点
  if (delNode->leftchild == nullptr && delNode->rightchild == nullptr) {
    if (parent != nullptr) {
      if (parent->leftchild == delNode)
        parent->leftchild = nullptr;
      else
        parent->rightchild = nullptr;
    }
  } else if (delNode->leftchild != nullptr && delNode->rightchild != nullptr) {
    // 删除该节点就是删除小于该节点的最大的那个节点，即左子树的右下节点
    parent = delNode;
    track.Push(parent);
    tmp = delNode->leftchild;
    while (tmp->rightchild != nullptr) {  // 找到最右下节点
      parent = tmp;
      track.Push(parent);
      tmp = tmp->rightchild;
    }
    delNode->data = tmp->data;  // 将右下节点的值复制到该节点
    delNode = tmp;              // 要删除的叶子节点

    if (parent->leftchild == delNode) {
      parent->leftchild = delNode->leftchild;
    } else {
      parent->rightchild = delNode->leftchild;
    }
  } else {
    if (delNode->leftchild != nullptr)
      tmp = delNode->leftchild;
    else
      tmp = delNode->rightchild;

    if (delNode->leftchild == delNode)
      parent->leftchild = tmp;
    else
      parent->rightchild = tmp;
  }

  // 弹栈处理平衡因子
  while (!track.IsEmpty()) {
    if (track.Pop(parent) == true) {
      // 删除的是叶子节点，并且parent只有这一个叶子
      if (parent->leftchild == nullptr && parent->rightchild == nullptr)
        parent->balfac = 0;
      else if (parent->leftchild == tmp)
        parent->balfac--;
      else
        parent->balfac++;

      if (parent->balfac == -1 || parent->balfac == 1) {
        break;  // parent之前的平衡因子是0
      }
      // 原来的平衡因子不为0，需要继续回溯调整
      if (parent->balfac == 0) {
        tmp = parent;
        continue;
      }
      // 找到最小不平衡树
      if (parent->balfac == -2) {
        // 最小不平衡树，说明之前是-1，一定有右孩子
        if (parent->rightchild->balfac == -1)  // RR 左旋转
          RotateLeft(parent);
        else if (parent->rightchild->balfac == 1)  // RL
          RotateRightLeft(parent);
        else if (parent->rightchild->balfac == 0) {
          RotateLeft(parent);
          parent->balfac = 1;
          parent->leftchild->balfac = -1;
        }
      } else if (parent->balfac == 2) {
        // 之前是1，一定有左孩子
        if (parent->leftchild->balfac == 1)  // LL
          RotateRight(parent);
        else if (parent->leftchild->balfac == -1)  // LR
          RotateLeftRight(parent);
        else if (parent->leftchild->balfac == 0) {
          RotateRight(parent);
          parent->balfac = -1;
          parent->rightchild->balfac = 1;
        }
      }
      cout << "调整: " << tmp->data << endl;
      if (track.IsEmpty()) {
        root = parent;
      } else {
        // 平衡性调整最小不平衡树会是根节点发生变化，让旧根节点的父节点指向新根节点
        AVLNode<T> *pParePoint = nullptr;
        track.GetTop(pParePoint);
        if (pParePoint->data > parent->data) {
          pParePoint->leftchild = parent;
        } else {
          pParePoint->rightchild = parent;
        }
      }
    }
  }
  delete delNode;
}

int main() {
  AVLTree<int> mybtr;
  // int array[] = {60, 40, 70, 120, 95};
  // int array[] = {12, 4, 1, 3, 7, 8, 10, 9, 2, 11, 6, 5};
  int array[] = {60, 31, 65, 15, 42, 62, 75, 12, 25, 37, 50,
                 63, 69, 85, 2,  32, 38, 48, 56, 82, 34};

  int acount = sizeof(array) / sizeof(int);
  for (int i = 0; i < acount; ++i) mybtr.InsertElem(array[i]);
  mybtr.DisplayIn();
  mybtr.DisplayPre();
  STRIP_LINE
  mybtr.DelElem(65);
  STRIP_LINE
  mybtr.DisplayIn();
  mybtr.DisplayPre();
  return 0;
}