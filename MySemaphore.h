#ifndef MYSEMAPHORE_H
#define MYSEMAPHORE_H

#include "MyQueue.h"
#include "MyScheduler.h"
#include <string.h>

using namespace std;

//************************************************************************************
//Purpose : To control access of resources provided in the window.
//************************************************************************************
class MySemaphore {
 public:
  MySemaphore (char *name) 
  {
      this->sema_value = 1; 
      strcpy(resource_name, name);
      this->sema_queue;
      dumping = false;
  }

  void set_dump_win(WINDOW *win);
  void set_sched_ptr(MyScheduler *ptr);
  void down(int);
  void up();
  void dump(int level);
  void un_dump();

 private:
  char resource_name[64];
  int sema_value;
  MyQueue<int> sema_queue;
  MyScheduler *Scheduler;
  MyVector<MyScheduler::TCB> states;
  bool dumping;
  WINDOW *dump_window;
};
#endif
