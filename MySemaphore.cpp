#include "MyQueue.h"
#include "MySemaphore.h"
#include <cstdio>
#include <ncurses.h>
#include <unistd.h>
#include <cstdarg>
#include <mutex>
#include <thread>
#include <pthread.h>
#include <cstring>

using namespace std;

 mutex mtx;

//************************************************************************************
//Purpose: Assigns the pointer to dump window.
//Input  : *win
//Output : none
//************************************************************************************
void MySemaphore::set_dump_win(WINDOW *win) {
  dump_window = win;
  Scheduler->setDumpWindow(win);
}

//************************************************************************************
//Purpose: Scheduler has the same address as ptr, to have the same access.
//Input  : *ptr
//Output : none
//************************************************************************************
void MySemaphore::set_sched_ptr(MyScheduler *ptr) {
    Scheduler = ptr;
}

//************************************************************************************
//Purpose : locks the mutex if the shared resource was free,
//          otherwise the calling thread is blocked and enqueued.
//Input   : Tid - thread id.
//Output  : none
//************************************************************************************
void MySemaphore::down(int Tid) {

    if(sema_value == 1)
    {
        sema_value = 0;
        mtx.lock();

    }
    else
    {
      Scheduler->yield(Tid);
      sema_queue.enqueue(Tid);

    }
}

//************************************************************************************
//Purpose: Sets state to ready when queue is not empty, outherwise it is blocked.
//         it also umlocks the mutex.
//Input  : none
//Output : none
//************************************************************************************
void MySemaphore::up() {
    if(!sema_queue.isEmpty())
    {
	int *intptr = sema_queue.dequeue();
	if (intptr)
	  Scheduler->change_state(*intptr, MyScheduler::READY);
    }
    else
    {
        sema_value = 1;
    }
    mtx.unlock();
}

//************************************************************************************
//Purpose  : Displays TCB structure values in a nice format on a window.
//Input    : level - gets called by another class to display the process state.
//Output   : none.
//************************************************************************************
void MySemaphore::dump(int level) {
    if (dumping)
	return;

    states.clear();

    MyScheduler::TCB *process;
    for (int i = 0; i < Scheduler->process_table.getLength(); i++) {
	process = Scheduler->process_table.at(i);
	states.insert(*process, states.getLength());
	process->state = MyScheduler::BLOCKED;
    }

    dumping = true;

    wprintw(dump_window, " \nResource: %s\n", resource_name);
    wprintw(dump_window, " Sema Value: %d\n", sema_value);

    MyQueue<int> tempQ;
    int *intptr;

    wprintw(dump_window, " Sema Queue: \n");

    if (!sema_queue.isEmpty()) {
      intptr = sema_queue.dequeue();
      tempQ.enqueue(*intptr);
      wprintw(dump_window, "%d", *intptr);

      for(int i = 0 ; i < sema_queue.getLength(); i++) {
	intptr = sema_queue.dequeue();
	tempQ.enqueue(*intptr);
        wprintw(dump_window, "-->%d", *intptr);
      }
    }

    Scheduler->dump(level);
}

//************************************************************************************
//Purpose: Assings the state values before dump was called.
//Input  : None
//Output : None
//************************************************************************************

void MySemaphore::un_dump() {
  for (int i = 0; i < Scheduler->process_table.getLength(); i++)
    Scheduler->process_table.at(i)->state = states.at(i)->state;

  dumping = false;
}
