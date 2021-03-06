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
  void setLogWindow(WINDOW *log_win);
  //void start_manage_tasks(void *ptr)
  static const int DEAD = -1, BLOCKED = 0, READY = 1, RUNNING = 2;
  void create_task(pthread_t *pt_t_ptr, void *(*fun) (void *), WINDOW *win, const char *name, int state);
  void destroy_task(int id);
  void yield(int id);
  void dump(int level);
  void garbage_collect();
  void change_state(int id, int state);
  void run_next(int id);

  struct TCB {
    int state, tid;
    char tname[64];
    WINDOW *win_ptr;
    WINDOW *log_win_ptr;;
  };
  MyVector<TCB> process_table;

private:
  int next_tid;
  WINDOW *dump_window;
  WINDOW *log_window;

  //static void* manage_tasks(void *ptr);
};

#endif
