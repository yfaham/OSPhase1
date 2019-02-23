//#include "MyScheduler.h"
#include "MyQueue.h"
#include <iostream>

using namespace std;

void* yolo(void *k) {
  cout << "hello";
}

// struct TCB {
//   int stuff;
// };

int main() {
  MyQueue<int> q;
  for (int i = 0; i < 3; i++) {
    q.enqueue(i);
  }

  for (int i = 0; i < 3; i++) {
    cout << *(q.dequeue(i)) << endl;
  }
  //  MyScheduler sched;
  //const char *c = "kk";
  //  for (int i = 0; i < 3; i++) {
  //    cout << "1" << endl;
  //    sched.create_task(yolo, c, -1);
  //  }

  return 0;
}
