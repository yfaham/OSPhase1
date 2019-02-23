#include <MyVector.h>

using namespace std;

class MyScheduler {
 public:
  static const int BLOCKED = -1, READY = 0, RUNNING = 1;
  create_task();
  destroy_task();
  yield();
  dump();
  garbage_collect();

  struct TCB {
    int state, tid;
    char *tname;
    pthread_t thread;
  };
  MyVector<TCB> process_table;
}
