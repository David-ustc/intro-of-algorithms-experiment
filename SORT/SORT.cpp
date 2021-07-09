#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define true 1
#define false 0 
typedef int ElemType;
typedef int KeyType;
typedef struct {
    ElemType *r;
    int len;
} SqTable;

void InitList(SqTable &L, int len) {
    // 0号单元不用
    L.r = (ElemType*)malloc((len+1)*sizeof(ElemType));
    L.len = len;
}

void CopyList(SqTable L, SqTable &newL) {
    newL.r = (ElemType*)malloc((L.len+1)*sizeof(ElemType));
    newL.len = L.len;
    memcpy(newL.r, L.r, (L.len+1)*sizeof(ElemType));
}

// 求一个整数的p次方
int intpow(int n, unsigned int p) {
    int res = 1;
    for (unsigned int i=0; i<p; ++i)
        res *= n;
    return res;
}

// 生成一个随机整数，其取值范围是[0, bound]
int randb(int bound) {
    int r = 0;
    unsigned int power = 0;
    do {
        r *= RAND_MAX;
        r += rand(); ++power;
    } while (intpow(RAND_MAX, power) < bound);
    return r % (bound+1);
}

// 随机打乱一个数组
void RandomShuffleList(SqTable L) {
    ElemType* array = L.r + 1; int n = L.len;
    for (int i=n-1; i>0; --i) {
        int j = randb(i); // 0<=j<=i
        ElemType tmp = array[i]; array[i] = array[j]; array[j] = tmp;
    }
}

void BubbleSort(SqTable &L) {
  bool change = true;
  int n=L.len;
  for (int i=n; i>=2 && change; --i) {
    // 这里我们设置一个标记，如果j从1到i-1循环中没有发生元素的互换
    // 说明整个序列已经是有序的了，无需考虑更小的i
    change = false;
    for (int j=1; j<=i-1; ++j) {
      if (L.r[j] > L.r[j+1]) {
        ElemType tmp = L.r[j]; L.r[j] = L.r[j+1]; L.r[j+1] = tmp;
        change = true;
      }
    }
  }
}

void InsertSortSub(SqTable &L, int low, int high) {
  // 这个子函数对L.r[low..high]做简单插入排序
  int j;
  for (int i=low+1; i<=high; ++i)
    if (L.r[i]< L.r[i-1]) {
      ElemType tmp = L.r[i];
      for (j=i-1; tmp< L.r[j]&& j>=low; --j)
        L.r[j+1] = L.r[j]; // 元素后移
      L.r[j+1] = tmp;      // 插入到合适位置
    }
}

void InsertSort(SqTable &L) {
  InsertSortSub(L, 1, L.len);
}

int Partition(SqTable &L, int low, int high) {
  // 选择一个枢轴，将L.r[low..high]分为两部分
  // 返回枢轴最后所在的位置，以便进一步划分
  // 划分以后，在枢轴之前（之后）的元素都小于（大于）或等于枢轴
  int pivotloc = low; // 枢轴可以任意选取，例如取第一个位置 注意：当选择第i个位置时，应先将第一个元素与第i个交换位置 
  ElemType tmp = L.r[pivotloc];
  KeyType pivotkey = tmp;
  while (low<high) {
    while (low<high && L.r[high]>=pivotkey) --high;
    L.r[low] = L.r[high];
    while (low<high && L.r[low]<=pivotkey) ++low;
    L.r[high] = L.r[low];
  }
  L.r[low] = tmp;
  return low;
}

void QSort(SqTable &L, int low, int high) {
  // 对L[low..high]进行快速排序
  if (low < high) {
    int pivotloc = Partition(L, low, high);
    QSort(L, low, pivotloc-1);
    QSort(L, pivotloc+1, high);
  }
}

void QuickSort(SqTable &L) {
  QSort(L, 1, L.len);
}

void HeapAdjust(SqTable &L, int s, int m) {
  // 已知L.r[s..m]中除了L.r[s]以外，都满足大顶堆的定义
  // 本函数通过调整，使得L.r[s..m]成为一个大顶堆
  ElemType tmp = L.r[s];
  for (int i=2*s; i<=m; i*=2) { // 每次向下一层
    if (i<m && L.r[i]<L.r[i+1]) ++i;
    if (tmp>= L.r[i]) break; // 已经找到合适的位置
    L.r[s] = L.r[i]; s = i; // 与孩子换位
  }
  L.r[s] = tmp;
}

void HeapSort(SqTable &L) {
  int i; ElemType tmp;
  for (i=L.len/2; i>0; --i)
    HeapAdjust(L, i, L.len); // 构造初始大顶堆
  for (i=L.len; i>1; --i) {
    tmp = L.r[i];
    L.r[i] = L.r[1];
    L.r[1] = tmp; // 将最大的关键字放到L.r[i]
    HeapAdjust(L, 1, i-1); // 对L.r[1..i-1]调用筛选法重新调整为堆
  }
}

void Merge(ElemType* Rs, ElemType* Rt, int s, int m, int t) {
  // 已知Rs[s..m]和Rs[m+1..t]都是有序表，将它们归并存储到Rt[s..t]
  int i,j,k;
  for (i=s, j=m+1, k=s; i<=m && j<=t; ++k) {
    if (Rs[i]<= Rs[j]) Rt[k] = Rs[i++];
    else Rt[k] = Rs[j++];
  }
  for (; i<=m; ++i, ++k) Rt[k] = Rs[i];
  for (; j<=t; ++j, ++k) Rt[k] = Rs[j];
}

void MSort(ElemType* Rs, ElemType* Rt, int low, int high) {
  if (low < high) {
    int mid = (low+high)/2;
    MSort(Rs, Rt, low, mid); MSort(Rs, Rt, mid+1, high);
    Merge(Rs, Rt, low, mid, high);
    for (int i=low; i<=high; ++i) Rs[i] = Rt[i];
  }
}

void MergeSort(SqTable &L) {
  ElemType* tmp = new ElemType[L.len+1];
  MSort(L.r, tmp, 1, L.len);
  delete []tmp;
}

int main() {
	printf ("if (N==1000000) various sort algorithms' timer as follows:\n");
  int N2 = 1000000;
  SqTable Lb;
  InitList(Lb, N2);
  for (int i=1; i<=N2; ++i) Lb.r[i] = i;
  RandomShuffleList(Lb);
  clock_t begin, end;
    // 排序算法1
  SqTable L1;
    /* CopyList(Lb, L1);
    begin = clock(); // 计时器开始
    BubbleSort(L1);
    end = clock(); // 计时器结束
    printf("BubbleSort time: %g seconds\n", (float)(end-begin)/CLOCKS_PER_SEC);*/
    // 排序算法2
  SqTable L2;
    CopyList(Lb, L2);
    begin = clock(); // 计时器开始
    QuickSort(L2);
    end = clock(); // 计时器结束
    printf("QuickSort time: %g seconds\n", (float)(end-begin)/CLOCKS_PER_SEC);
    // 排序算法3
	SqTable L3;
	  CopyList(Lb, L3);
    begin = clock(); // 计时器开始
    HeapSort(L3);
    end = clock(); // 计时器结束
    printf("HeapSort time: %g seconds\n", (float)(end-begin)/CLOCKS_PER_SEC);
    // 排序算法4
	SqTable L4;
	  CopyList(Lb, L4);
    begin = clock(); // 计时器开始
    MergeSort(L4);
    end = clock(); // 计时器结束
    printf("MergeSort time: %g seconds\n", (float)(end-begin)/CLOCKS_PER_SEC);
    // 排序算法5 
    SqTable L5;
	/*CopyList(Lb, L5);
    begin = clock(); // 计时器开始
    InsertSort(L5);
    end = clock(); // 计时器结束
    printf("InserteSort time: %g seconds\n", (float)(end-begin)/CLOCKS_PER_SEC);*/
}
