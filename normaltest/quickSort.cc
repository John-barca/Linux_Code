#include <iostream>
using namespace std;

int partition(int* array, int leftIndex, int rightIndex) {
  int beginIndex = leftIndex;
  int endIndex = rightIndex;
  while (beginIndex < endIndex) {
    while (beginIndex < endIndex && array[beginIndex] <= array[rightIndex]) {
      beginIndex++;
    }
    while (beginIndex < endIndex && array[endIndex] >= array[rightIndex]) {
      endIndex--;
    }
    swap(array[beginIndex], array[endIndex]);
  }
  swap(array[beginIndex], array[rightIndex]);
  return beginIndex;
}

void _quickSort(int* array, int leftIndex, int rightIndex) {
  if (leftIndex >= rightIndex) {
    return;
  }
  int div = partition(array, leftIndex, rightIndex);
  _quickSort(array, leftIndex, div - 1);
  _quickSort(array, div + 1, rightIndex);
}

void quickSort(int* array, int sz) {
  _quickSort(array, 0, sz - 1);
}

int main() {
  int array[] = {2, 10, 7, 1, 3, 8, 5};
  int sz = sizeof(array) / sizeof(int);
  quickSort(array, sz);
  for (int i = 0; i < sz; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
  return 0;
}