#include "MyQueue.h"
#include <cstdio>
#include <ncurse.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdarg>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;  // mutex for critical section

MySemaphore::down(int Tid) {
    
    if(sema_value == 1)
        sema_value--;
    else
    {
        push(Tid);
        Myschedule->(setState, Tid, Block);
        SchedPtr->process_table.at(index)->state
        
    }
}

MySemaphore::up() {
    sema_value++;
}

MySemaphore::dump() {
    
}
