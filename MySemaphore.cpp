//************************************************************************************
//Purpose : To control access of resources provided
//************************************************************************************
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
//Purpose: Creates a dump window.
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
//Purpose : allows states to be blocked if they were ready otherwise it the Tid
//          control is let go, so the next tid can begin.
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
//Purpose  : Displays TCB structure values in a nice format.
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

    mvwprintw(dump_window, 1, 1, "Resource: %s", resource_name);
    mvwprintw(dump_window, 1, 1, "Sema Value: %d", sema_value);

    MyQueue<int> tempQ;
    int *intptr;
    
    mvwprintw(dump_window, 1, 1, "Sema Queue: ");

    if (sema_queue.isEmpty())
	return;

    intptr = sema_queue.dequeue();
    tempQ.enqueue(*intptr);
    wprintw(dump_window, "%d", *intptr);

    for(int i = 0 ; i < sema_queue.getLength(); i++) {
	intptr = sema_queue.dequeue();
	tempQ.enqueue(*intptr);
        wprintw(dump_window, "-->%d", *intptr);
    }
    
    Scheduler->dump(level);
}

//************************************************************************************
//Purpose: Assings the next state value.
//Input  : None
//Output : None
//************************************************************************************
void MySemaphore::un_dump() {
  for (int i = 0; i < Scheduler->process_table.getLength(); i++)
    Scheduler->process_table.at(i)->state = states.at(i)->state;

  dumping = false;
}
