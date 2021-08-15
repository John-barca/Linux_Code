#include <iostream>
#include <cassert>
using namespace std;

void adjustDown(int* array, int sz, int root) {
  while (1) {
    assert(array != NULL);
    if (root > sz || root < 0) {
      return;
    }
    int leftIndex = 2 * root + 1;
    if (leftIndex >= sz) {//叶子节点
      return;
    }
    int rightIndex = 2 * root + 2;
    int min = leftIndex;
    if (rightIndex < sz && array[rightIndex] < array[leftIndex]) {
      min = rightIndex;
    }
    if (array[root] <= array[min]) {
      return;
    }
    swap(array[root], array[min]);
    root = min;
  }
}

void craeteHeap(int* array, int sz) {
  // 找到最后一个叶子结点的父节点，开始向下调整
  for (int i = (sz - 2) / 2; i >= 0; i--) {
    adjustDown(array, sz, i);
  }
}

void heapSort(int* array, int sz) {
  craeteHeap(array, sz);
  for (int i = 0; i < sz; i++) {
    swap(array[0], array[sz - 1 - i]);
    adjustDown(array, sz - i - 1, 0);
  }
}

int main () {
  int array[] = {20, 17, 4, 16, 5, 3};
  int sz = sizeof(array) / sizeof(int);
  heapSort(array, sz);
  for(int i = 0; i < sz; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
  return 0;
}