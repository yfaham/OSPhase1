#ifndef MYSEMAPHORE_H
#define MYSEMAPHORE_H

#include "MyQueue.h"
#include "MyScheduler.h"

using namespace std;

class MySemaphore {
 public:
  void down(int);
  void up();
  void dump();

 private:
  char *resource_name;
  int sema_value;
  MyQueue<int> sema_queue;
  MyScheduler *Scheduler;
  mutex mtx;  // mutex for critical section
};

#endif
