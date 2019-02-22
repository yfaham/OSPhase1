#include <cstdio>
#include <ncurse.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdarg>

using namespace std;

MyScheduler::create_task(char *name, int state = BLOCKED) {
  TCB process = new TCB;
  process.state = BLOCKED;
  process.tid = process_table.getLength();
  sprintf(process.tname, name);
  pthread_create(&((process_table.at(process_table.getLength()))->data->thread), NULL, /*function*/,
		 &(process_table.at(process_table.getLength())->data));
}

MyScheduler::destroy_task(int tid) {

}

MyScheduler::yield() {
  pthread_yield();
}

MyScheduler::dump(WINDOW *win) {
  
}

MyScheduler::garbage_collect() {

}
