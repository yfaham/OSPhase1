#include "MyQueue.h"

using namespace std;

class MySemaphore {
 public:
  down(int);
  up();
  dump();

 private:
  char *resource_name;
  int sema_value;
  MyQueue sema_queue;
}
