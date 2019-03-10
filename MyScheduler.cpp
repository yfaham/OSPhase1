#define _OPEN_THREADS
#define _GNU_SOURCE
#include "MyScheduler.h"
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
#include <cstring>

using namespace std;

MyScheduler::MyScheduler() {
  next_tid = 0;
}

void MyScheduler::setDumpWindow(WINDOW *d_window) {
  dump_window = d_window;
}

void MyScheduler::setLogWindow(WINDOW *log_win) {
  log_window = log_win;
}

/*void MyScheduler::start_manage_tasks(void *ptr) {
  pthread_t pt;
  pthread_create(&pt, NULL, manage_tasks, ptr);
}*/

//************************************************************************************
//Purpose: Starts the task passed in the parameter as a thread
//Input  : pt_t_ptr: thread_t address
//         (*fun): pointer to the passed funtion
//         win: pointer to the corresponding window
//         name: of the thread
//         state: initial state of the thread
//Output : none
//************************************************************************************
void MyScheduler::create_task(pthread_t *pt_t_ptr, void *(*fun) (void *), WINDOW *win, const char *name, int state) {
  process_table.insert(*(new TCB), process_table.getLength());
  TCB *process = process_table.at(process_table.getLength() - 1);
  process->state = state;
  process->tid = next_tid++;
  process->win_ptr = win;
  process->log_win_ptr = log_window;
  strcpy(process->tname, name);
  pthread_create(pt_t_ptr, NULL, fun, process);
}

//************************************************************************************
//Purpose: marks the thread with tid == id as DEAD
//Input  : id
//Output : none
//************************************************************************************
void MyScheduler::destroy_task(int id) {
  for (int i = 0; i < process_table.getLength(); i++) {
    if (process_table.at(i)->tid == id) {
      process_table.at(i)->state = DEAD;
      break;
    }
  }
}

//************************************************************************************
//Purpose: To set the thread with tid == id to BLOCKED
//Input  : id
//Output : none
//************************************************************************************
void MyScheduler::yield(int id) {
  //for (int i = 0; i < process_table.getLength(); i++)
  //if (process_table.at(i)->tid == id)
  //process_table.at(i)->state = BLOCKED;
  pthread_yield();
}

//************************************************************************************
//Purpose: Prints the dump, content on the window, with details based on level.
//Input  : level
//Output : none
//************************************************************************************
void MyScheduler::dump(int level) {
  wprintw(dump_window, " \n Scedhuler dump:\n");
  TCB *process;
  for (int i = 0; i < process_table.getLength(); i++) {
    process = process_table.at(i);
    wprintw(dump_window, " Name = %s", process->tname);

    if (level >= 1) {
      wprintw(dump_window, ", TID = %d", process->tid);
    }
    if (level >= 2) {
      switch (process->state) {
      case DEAD:
	wprintw(dump_window, ", state = DEAD");
        break;
      case BLOCKED:
	wprintw(dump_window, ", state = BLOCKED");
        break;
      case READY:
	wprintw(dump_window, ", state = READY");
        break;
      case RUNNING:
	wprintw(dump_window, ", state = RUNNING");
        break;
      }
    }
    wprintw(dump_window, "\n");
    box(dump_window, 0, 0);
    wrefresh(dump_window);
  }
}

//************************************************************************************
//Purpose: Removes threads that are marked DEAD
//Input  : none
//Output : none
//************************************************************************************
void MyScheduler::garbage_collect() {
  for (int i = 0; i < process_table.getLength(); i++)
    if (process_table.at(i)->state == DEAD)
      process_table.remove(i);
}

//************************************************************************************
//Purpose: changes state of the thread with tid == id passed in.
//Input  : id, newState.
//Output : none
//************************************************************************************
void MyScheduler::change_state(int id, int newState) {
  for (int i = 0; i < process_table.getLength(); i++)
    if (process_table.at(i)->tid == id)
      process_table.at(i)->state = newState;
}

//************************************************************************************
//Purpose: Runs the next id by implicitly blocking the currrent id
//Input  : id
//Output : none
//************************************************************************************
void MyScheduler::run_next(int id) {
  int i;
  for (i = 0; i < process_table.getLength(); i++) {
    if (process_table.at(i)->tid == id) {
      process_table.at(i)->state = READY;
      break;
    }
  }
  
  i = (i + 1) % process_table.getLength();

  while (process_table.at(i)->state != READY)
    i = (i + 1) % process_table.getLength();

  process_table.at(i)->state = RUNNING;
}


/*void* MyScheduler::manage_tasks(void *ptr) {
  int i = 0;
  MyScheduler *sched_ptr = (MyScheduler*) ptr;
  TCB *process;
  while (true) {
    if (!sched_ptr->process_table.isEmpty()) {
      process = sched_ptr->process_table.at(i);
      
      if (process->state == READY) {
        process->state = RUNNING;
	i = (i + 1) % sched_ptr->process_table.getLength();
      }
      else if (process->state == RUNNING)
	pthread_yield();
      else
        i = (i + 1) % sched_ptr->process_table.getLength();
    }
    else
      pthread_yield();
  }
  return NULL;
}*/
