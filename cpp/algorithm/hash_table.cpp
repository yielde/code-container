// 哈希函数通过除留余数法
// 冲突解决使用链表法

#include "publiclib.h"

namespace _myhashspace {
#define MAX_HASHTABLE_IZE 40

template <typename T>
struct ListNode {
  T data;
  ListNode<T> *next;
};

template <typename T>
class myHashTbl {
 public:
  myHashTbl() {
    for (int i = 0; i < MAX_HASHTABLE_IZE; ++i) {
      m_BucketArray[i] = nullptr;
    }
  }
  ~myHashTbl() {
    for (int i = 0; i < MAX_HASHTABLE_IZE; ++i) {
      if (m_BucketArray[i] == nullptr) continue;

      ListNode<T> *node = m_BucketArray[i];
      ListNode<T> *tmp;

      while (node != nullptr) {
        tmp = node;
        node = node->next;
        delete tmp;
      }
    }
    cout << "析构~" << endl;
  }

  void DspHashTbl() {
    for (int i = 0; i < MAX_HASHTABLE_IZE; ++i) {
      if (m_BucketArray[i] == nullptr) {
        cout << "[桶 " << i << "] = nullptr" << endl;
        continue;
      }
      cout << "[桶 " << i << "] = ";
      ListNode<T> *node = m_BucketArray[i];
      while (node != nullptr) {
        cout << node->data << " ";
        node = node->next;
      }
      cout << endl;
    }
  }

  void Add(T key) {
    int idx = key % MAX_HASHTABLE_IZE;
    if (inTable(m_BucketArray[idx], key)) {
      cout << key << "在桶[" << idx << "]中" << endl;
      return;
    }

    ListNode<T> *newNode = new ListNode<T>;
    newNode->data = key;

    newNode->next = m_BucketArray[idx];
    m_BucketArray[idx] = newNode;
    cout << "在桶[" << idx << "] 插入" << key << endl;
  }

  void Delete(T key) {
    int idx = key % MAX_HASHTABLE_IZE;
    if (m_BucketArray[idx] == nullptr) {
      cout << "桶[" << idx << "]没有值" << endl;
      return;
    }
    ListNode<T> *parent = nullptr;
    ListNode<T> *currNode = m_BucketArray[idx];

    while (currNode != nullptr) {
      if (currNode->data == key) {
        if (parent == nullptr)
          m_BucketArray[idx] = currNode->next;
        else
          parent->next = currNode->next;
        delete currNode;
        return;
      }
      parent = currNode;
      currNode = currNode->next;
    }
    cout << "桶[" << idx << "]中没有" << key << endl;
  }

  bool Find(T key) {
    int idx = key % MAX_HASHTABLE_IZE;
    bool ifind = inTable(m_BucketArray[idx], key);
    if (ifind) {
      cout << key << "在桶[" << idx << "]中" << endl;

    } else {
      cout << "没有找到" << endl;
    }
    return ifind;
  }

  bool inTable(ListNode<T> *head, T key) {
    ListNode<T> *tmp = head;
    while (tmp) {
      if (tmp->data == key) return true;
      tmp = tmp->next;
    }
    return false;
  }

 private:
  ListNode<T> *m_BucketArray[MAX_HASHTABLE_IZE];
};

}  // namespace _myhashspace

int main() {
  _myhashspace::myHashTbl<int> myhashtbl;
  for (int i = 0; i < 60; ++i) {
    myhashtbl.Add(i);
  }
  myhashtbl.DspHashTbl();
  STRIP_LINE
  myhashtbl.Find(55);
  STRIP_LINE

  myhashtbl.Find(21);
  STRIP_LINE

  for (int i = 70; i > 35; --i) {
    myhashtbl.Delete(i);
  }
  STRIP_LINE
  myhashtbl.DspHashTbl();
  return 0;
}