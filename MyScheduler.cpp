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

void MyScheduler::create_task(void *(*fun) (void *), WINDOW *win, const char *name, int state) {
  process_table.insert(*(new TCB), process_table.getLength());
  TCB *process = process_table.at(process_table.getLength() - 1);
  process->state = BLOCKED;
  process->tid = next_tid++;
  process->win_ptr = win;
  strcpy(process->tname, name);
  pthread_create(&(process->thread), NULL, fun, process);
}

void MyScheduler::destroy_task(int id) {
  for (int i = 0; i < process_table.getLength(); i++) {
    if (process_table.at(i)->tid == id) {
      process_table.at(i)->state = DEAD;
      break;
    }
  }
}

void MyScheduler::yield(int id) {
  for (int i = 0; i < process_table.getLength(); i++)
    if (process_table.at(i)->tid == id)
      process_table.at(i)->state = BLOCKED;
}

void MyScheduler::dump(int level) {
  TCB *process;
  for (int i = 0; i < process_table.getLength(); i++) {
    char info[64] = "Name = ";
    process = process_table.at(i);
    strcat(info, process->tname);
    if (level >= 1) {
      char tid[6] = {0};
      sprintf(tid, ", TID = %d", process->tid);
      strcat(info, tid);
    }
    if (level >= 2) {
      switch (process->state) {
        case DEAD:
        strcat(info, "state = DEAD");
        break;
        case BLOCKED:
        strcat(info, "state = BLOCKED");
        break;
        case READY:
        strcat(info, "state = READY");
        break;
        case RUNNING:
        strcat(info, "state = RUNNING");
        break;
      }
    }
    mvwprintw(dump_window, 1, 1, info);
    box(dump_window, 0, 0);
    wrefresh(dump_window);
  }
}

void MyScheduler::garbage_collect() {
  for (int i = 0; i < process_table.getLength(); i++)
    if (process_table.at(i)->state == DEAD)
      process_table.remove(i);
}

void MyScheduler::change_state(int id, int newState) {
  for (int i = 0; i < process_table.getLength(); i++)
    if (process_table.at(i)->tid == id)
      process_table.at(i)->state = newState;
}

void MyScheduler::manage_tasks() {
  int i = 0;
  TCB *process;
  while (true) {
    if (process_table.isEmpty())
      pthread_yield();
    else {
      process = process_table.at(i);
      if (process->state == RUNNING)
        pthread_yield();
      else if (process->state == READY)
        process->state = RUNNING;
      else if (process->state == BLOCKED || process->state == DEAD)
        i = (i + 1) % process_table.getLength();
    }
  }
}
