#include <iostream>
using namespace std;

void selectSort(int* array, int sz) {
  for (int i = 0; i < sz; i++) {
    for (int j = i + 1; j < sz; j++) {
      if (array[i] > array[j]) {
        swap(array[i], array[j]);
      }
    }
  }
}

int main () {
  int array[] = {4, 2, 7, 5, 1, 3};
  int size = sizeof(array) / sizeof(int);
  selectSort(array, size);
  for (int i = 0; i < size; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
  return 0;
}