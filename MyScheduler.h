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

 private:
  struct TCB {
    int state, tid;
    char *tname;
  };
  MyVector<TCB> process_table;
}
