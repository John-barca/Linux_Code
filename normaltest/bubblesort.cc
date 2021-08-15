#include <iostream>
using namespace std;

// 冒泡
void bubbleSort(int* array, int sz) {
  for (int i = 0; i < sz - 1; i++) {
    int flag = 0;
    for (int j = 0; j < sz - 1 - i; j++) {
      if (array[j] > array[j + 1]) {
        flag = 1;
        swap(array[j], array[j + 1]);
      }
    }
    if (!flag) {
      break;
    }
  }
}

int main() {
  int array[6] = {2, 1, 4, 3, 9, 5};
  int sz = sizeof(array) / sizeof(int);
  bubbleSort(array, sz);
  for (int i = 0; i < sz; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
  
  return 0;
}