//#include "MyScheduler.h"
#include "MyVector.h"
#include <iostream>

using namespace std;

void* yolo(void *k) {
  cout << "hello";
}

struct TCB {
  int stuff;
};

int main() {
  MyVector<TCB> v;
  for (int i = 0; i < 3; i++) {
    v.insert(*(new TCB), v.getLength());
    cout << v.getLength() << endl;
    cout << v.at(v.getLength() - 1)->stuff << endl;
  }
  //  MyScheduler sched;
  //const char *c = "kk";
  //  for (int i = 0; i < 3; i++) {
  //    cout << "1" << endl;
  //    sched.create_task(yolo, c, -1);
  //  }

  return 0;
}
