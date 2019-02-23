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

using namespace std;

void MyScheduler::create_task(void *(*start_routine) (void *), const char *name, int state) {
  process_table.insert(*(new TCB), process_table.getLength());
  TCB *process = process_table.at(process_table.getLength() - 1);
  process->state = BLOCKED;
  process->tid = process_table.getLength();
  sprintf(process->tname, name);
  cout << "hey" << endl;
  pthread_create(&(process->thread), NULL, start_routine,
		 process_table.at(process_table.getLength()));
}

void MyScheduler::destroy_task(int id) {
  for (int i = 0; i < process_table.getLength(); i++) {
    if (process_table.at(i)->tid == id) {
      process_table.at(i)->state = DEAD;
      break;
    }
  }
}

void MyScheduler::yield() {
  pthread_yield();
}

void MyScheduler::dump(WINDOW *win) {
  TCB *temp;
  for (int i = 0; i < process_table.getLength(); i++) {
    temp = process_table.at(i);
    cout << temp->state << endl;
    cout << temp->tid << endl;
    cout << temp->tname << endl;
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
