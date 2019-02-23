#include "MyQueue.h"
#include "MySemaphore.h"
#include <cstdio>
#include <ncurses.h>
#include <unistd.h>
#include <cstdarg>
#include <mutex>
#include <thread>
#include <pthread.h>

using namespace std;

MyQueue<int> q;

mutex mtx;  // mutex for critical section

void MySemaphore::down(int Tid) {
    
    if(sema_value == 1)
    {
        mtx.lock();
        sema_value--;
    }
    else
    {
        q.enqueue(Tid);
        
        for (int i = 0; i < Scheduler->process_table.getLength(); i++) {
            Scheduler->process_table.at(i)->state = Scheduler->BLOCKED;
        }
    }
}

void MySemaphore::up() {
    
    if(!q.isEmpty())
    {
        q.dequeue();
        mtx.unlock();
    }
    else
    {
        sema_value++;
        mtx.unlock();
    }
}

void MySemaphore::dump() {
    
    cout << "Resource: " << resource_name;
    //for(int i =0 ; i < q.getLength(); i++)
      //  cout << *(resource_name);
    Scheduler -> yield();
    
    cout << "Sema_value: " << sema_value << endl;
    Scheduler -> yield();
    
    cout << "Sema_queue: ";
    for(int i = 0 ; i < q.getLength(); i++)
    {
        cout << sema_queue.front() << " ";
        sema_queue.dequeue();
    }
    Scheduler -> yield();
}
