//#include "MyScheduler.h"
#include "MyScheduler.h"
#include <iostream>

using namespace std;

struct TCB {
  int stuff;
};

int main() {
  MyScheduler sched;
  const char *c = "kk";
   for (int i = 0; i < 3; i++) {
     cout << "1" << endl;
     sched.create_task(yolo, c, -1);
   }

  return 0;
}
