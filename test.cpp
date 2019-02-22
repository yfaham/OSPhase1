#include "MyVector.h"
#include <iostream>

using namespace std;

int main() {
  MyVector<int> v;
  
  for (int i = 0; i < 10; i++) {
    v.insert(i, v.getLength());
  }

  for (int i = 0; i < 10; i++) {
    cout << v.at(i) << endl;
  }

  cout << v.getLength() << endl;

  for (int i = 0; i < 10; i++) {
    cout << v.at(0) << endl;
    v.remove(0);
  }

  return 0;
}
