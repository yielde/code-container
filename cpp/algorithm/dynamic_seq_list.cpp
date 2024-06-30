#include <iostream>
#define INIT_LEN 10
#define INCRASE_LEN 5
using std::cout, std::endl;

template <typename T>
class SeqList {
 public:
  SeqList(int length = INIT_LEN);
  ~SeqList();

 public:
  bool ListInsert(int i,
                  const T& value);  // 插入元素，元素在顺序列表中的位置为第i个
  bool ListDelete(int i);
  bool GetElem(int i, T& value);  // 获取第i个位置的元素值
  int LocateElemFirst(const T& value);

  void DisplayList();  // 输出顺序列表中所有元素
  int ListLength();    // 列表长度
  void ReverseList();  // 反转顺序列表

 private:
  void IncreaseSize();  // 数据存储满后扩容

 private:
  T* m_data;      // 顺序列表地址
  int m_length;   // 列表长度
  int m_maxsize;  // 顺序列表最大容量
};

template <typename T>
SeqList<T>::SeqList(int length) {
  m_data = new T[length];
  m_length = 0;
  m_maxsize = length;
}

template <typename T>
SeqList<T>::~SeqList() {
  T* tmp = m_data;
  m_data = nullptr;
  m_length = 0;
  delete[] tmp;
}

template <typename T>
bool SeqList<T>::ListInsert(int i, const T& value) {
  if (i < 1 || i > m_length + 1) {
    cout << "插入元素的位置不正确" << endl;
    return false;
  }

  if (m_length >= m_maxsize) {
    IncreaseSize();
  }

  for (int j = m_length; j > i - 1; --j) {
    m_data[j] = m_data[j - 1];
  }
  m_data[i - 1] = value;
  cout << "插入元素 " << value << " 到位置 " << i << endl;
  m_length += 1;
  return true;
}

template <typename T>
bool SeqList<T>::ListDelete(int i) {
  if (m_length < 1) {
    cout << "列表为空，不执行删除操作" << endl;
    return false;
  }
  if (i < 1 || i > m_length) {
    cout << "该位置没有元素，不执行删除操作" << endl;
    return false;
  }

  for (int j = i; j < m_length; ++j) {
    m_data[j - 1] = m_data[j];
  }
  cout << "删除第" << i << "个元素" << endl;
  m_length -= 1;
  return true;
}

template <typename T>
bool SeqList<T>::GetElem(int i, T& value) {
  if (i < 1 || i > m_length) {
    cout << "该元素不存在" << endl;
    return false;
  }
  value = m_data[i - 1];
  return true;
}

template <typename T>
int SeqList<T>::LocateElemFirst(const T& value) {
  for (int i = 0; i < m_length; ++i) {
    if (m_data[i] == value) {
      cout << "元素" << value << "首次出现在第" << i + 1 << "个位置" << endl;
      return i + 1;
    }
  }
  cout << "元素" << value << "不在列表中" << endl;
  return 0;
}

template <typename T>
void SeqList<T>::DisplayList() {
  cout << "该列表中的元素有: ";
  for (int i = 0; i < m_length; ++i) {
    cout << m_data[i] << " ";
  }
  cout << endl;
}

template <typename T>
int SeqList<T>::ListLength() {
  return m_length;
}

template <typename T>
void SeqList<T>::ReverseList() {
  if (m_length <= 1) {
    cout << "元素小于2个，无需翻转" << endl;
  }
  T temp;
  for (int i = 0; i < m_length / 2; ++i) {
    temp = m_data[m_length - i - 1];
    m_data[m_length - i - 1] = m_data[i];
    m_data[i] = temp;
  }
  cout << "反转后的列表为: ";
  DisplayList();
}

template <typename T>
void SeqList<T>::IncreaseSize() {
  T* tmp = m_data;
  m_data = new T[m_length + INCRASE_LEN];
  for (int i = 0; i < m_length; ++i) {
    m_data[i] = tmp[i];
  }
  m_maxsize = m_length + INCRASE_LEN;
  cout << "数组长度从 " << m_length << " 增长到 " << m_maxsize;
  delete[] tmp;
}

int main() {
  SeqList<char> char_seq;
  bool ret = char_seq.ListInsert(1, 'a');
  ret = char_seq.ListInsert(2, 'c');
  ret = char_seq.ListInsert(2, 'b');
  ret = char_seq.ListInsert(1, 'd');
  char_seq.DisplayList();
  char_seq.ReverseList();
  cout << endl;

  char_seq.ListDelete(1);
  char_seq.ListDelete(1);
  char_seq.DisplayList();
  cout << endl;

  char value = '\0';
  int locate = 2;
  char_seq.GetElem(locate, value);
  cout << "当前列表中第 " << locate << " 个元素的值为" << value << endl;
  cout << endl;

  char_seq.LocateElemFirst('c');
  cout << endl;

  int len = char_seq.ListLength();
  cout << "当前列表长度为 " << len << endl;
  cout << endl;

  char_seq.ReverseList();

  for (int i = 1; i < 10; ++i) {
    char_seq.ListInsert(i, 'o');
  }
  char_seq.DisplayList();
  len = char_seq.ListLength();
  cout << "列表长度为: " << len << endl;
  cout << endl;
  return 0;
}
