#ifndef MYSCHEDULER_H
#define MYSCHEDULER_H

#define _OPEN_THREADS
#define _GNU_SOURCE
#include "MyVector.h"
#include <iostream>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdarg.h>
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>

using namespace std;

class MyScheduler {
public:
  MyScheduler();
  void setDumpWindow(WINDOW *d_window);
  static const int DEAD = -1, BLOCKED = 0, READY = 1, RUNNING = 2;
  void create_task(void *(*fun) (void *), WINDOW *win, const char *name, int state);
  void destroy_task(int id);
  void yield(int id);
  void dump(int level);
  void garbage_collect();
  void change_state(int id, int state);

  struct TCB {
    int state, tid;
    char tname[64];
    pthread_t thread;
    WINDOW *win_ptr;
  };
  MyVector<TCB> process_table;

private:
  int next_tid;
  WINDOW *dump_window;
  void manage_tasks(void *ptr);
  //run_task(void *(*fun) (void *), void *args, const char *name, int state);
};

#endif
