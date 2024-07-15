#include <climits>
#include <map>
#include <string>

#include "publiclib.h"

struct HFMTreeNode {
  int weight;
  int parent;
  int lchild;
  int rchild;
};

class HFMTree {
 public:
  /// @brief
  /// @param nodecount 要生成huffman树的叶子节点个数
  /// @param pweight 每个叶子节点的权重数组
  HFMTree(int nodecount, int *pweight) {
    m_length = nodecount;
    MaxNodeCount = 2 * m_length - 1;  // n个叶子节点会生成 n-1个新节点，n-1
                                      // + n = 2n - 1，最终的树节点数为2n-1
    m_data = new HFMTreeNode[MaxNodeCount];

    for (int i = 0; i < MaxNodeCount; ++i) {
      m_data[i].parent = -1;
      m_data[i].lchild = -1;
      m_data[i].rchild = -1;
    }
    for (int i = 0; i < m_length; ++i) {
      m_data[i].weight = pweight[i];  // 先将节点放入树中
    }
    chartoIdxMap['A'] = 0;
    chartoIdxMap['B'] = 1;
    chartoIdxMap['C'] = 2;
    chartoIdxMap['D'] = 3;
    chartoIdxMap['E'] = 4;
    chartoIdxMap['F'] = 5;

    IdxtoCharMap[0] = 'A';
    IdxtoCharMap[1] = 'B';
    IdxtoCharMap[2] = 'C';
    IdxtoCharMap[3] = 'D';
    IdxtoCharMap[4] = 'E';
    IdxtoCharMap[5] = 'F';
  }

  ~HFMTree() { delete[] m_data; }

  void CreateHFMTree() {
    int idx1 = 0;
    int idx2 = 0;

    int initlength = m_length;
    for (int i = initlength; i < MaxNodeCount; ++i) {
      TwoMinValue(idx1, idx2);
      m_data[i].weight = m_data[idx1].weight + m_data[idx2].weight;
      m_data[i].lchild = idx1;
      m_data[i].rchild = idx2;

      m_data[idx1].parent = i;
      m_data[idx2].parent = i;
      m_length++;
    }
  }

  void preOrder(int idx) {
    if (idx != -1) {
      cout << m_data[idx].weight << " ";
      preOrder(m_data[idx].lchild);
      preOrder(m_data[idx].rchild);
    }
  }

  int GetLength() { return m_length; }

  // huffman编码, idx表示节点在数组中的下标
  bool CreateHFMCode(int idx) {
    int leafNodeCount = (MaxNodeCount + 1) / 2;
    if (idx < 0 || idx >= leafNodeCount) {
      // 只能对叶子节点求huffman编码
      return false;
    }
    std::string result = "";
    int curridx = idx;
    int tmpparent = m_data[curridx].parent;
    // 路径向左为0，路径向右为1
    while (tmpparent != -1) {
      if (m_data[tmpparent].lchild == curridx) {
        result = "0" + result;
      } else {
        result = "1" + result;
      }
      curridx = tmpparent;
      tmpparent = m_data[curridx].parent;
    }
    cout << "下标为【" << idx << "】，权值为" << m_data[idx].weight
         << "的节点的哈夫曼编码是" << result << endl;
    return true;
  }

  // HFMTreeNode *hfmtree() { return m_data; }
  std::string Eecode(const std::string s) {
    std::string result = "";
    for (auto it : s) {
      std::string ss = "";
      if (chartoIdxMap.count(it) > 0) {
        int curridx = chartoIdxMap[it];
        int tempParent = m_data[curridx].parent;
        while (tempParent != -1) {
          if (m_data[tempParent].lchild == curridx)
            ss = "0" + ss;
          else
            ss = "1" + ss;

          curridx = tempParent;
          tempParent = m_data[curridx].parent;
        }
      }
      result += ss;
    }
    return result;
  }

  std::string Decode(const std::string bs) {
    std::string result = "";
    int index = 0;
    int tmpidx = -1;
    int count = MaxNodeCount - 1;
    while (index < bs.length()) {
      int tmpidx = atoi(bs.substr(index, 1).c_str());
      if (tmpidx == 0)
        count = m_data[count].lchild;
      else
        count = m_data[count].rchild;

      if (m_data[count].lchild == -1 &&
          m_data[count].rchild == -1) {  // 叶子节点
        result = result + IdxtoCharMap[count];
        count = MaxNodeCount - 1;
      }
      index++;
    }
    return result;
  }

 private:
  void TwoMinValue(int &idx1, int &idx2) {
    int minval1 = INT_MAX, minval2 = INT_MAX;
    for (int i = 0; i < m_length; ++i) {
      if (m_data[i].parent == -1)  // 节点未使用
      {
        if (minval1 > m_data[i].weight) {
          minval1 = m_data[i].weight;
          idx1 = i;
        }
      }
    }

    for (int i = 0; i < m_length; ++i) {
      if (m_data[i].parent == -1 && i != idx1) {
        if (minval2 > m_data[i].weight) {
          minval2 = m_data[i].weight;
          idx2 = i;
        }
      }
    }
  }

 private:
  HFMTreeNode *m_data;
  int m_length;  // 记录当前的树有多少个下标
  int MaxNodeCount;
  std::map<char, int> chartoIdxMap;
  std::map<int, char> IdxtoCharMap;
};

std::map<int, char> CMap() {
  std::map<int, char> tmp;
  tmp[0] = 'A';
  tmp[1] = 'B';
  tmp[2] = 'C';
  tmp[3] = 'D';
  tmp[4] = 'E';
  tmp[5] = 'F';
  return tmp;
}

int main() {
  int weighLst[] = {12, 15, 9, 24, 8, 32};
  HFMTree hfmobj(sizeof(weighLst) / sizeof(int), weighLst);
  hfmobj.CreateHFMTree();
  hfmobj.preOrder(hfmobj.GetLength() - 1);
  STRIP_LINE
  for (int i = 0; i < sizeof(weighLst) / sizeof(weighLst[0]); ++i)
    hfmobj.CreateHFMCode(i);
  STRIP_LINE
  std::string A = hfmobj.Eecode("ABCDEFDEEF");
  cout << A << endl;
  STRIP_LINE

  std::string B = hfmobj.Decode("10010100101000110100000011");
  cout << B << endl;
  STRIP_LINE
  return 0;
}
