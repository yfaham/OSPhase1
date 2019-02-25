#include <ncurses.h>
#include <unistd.h>
#include <stdarg.h>
#include <termios.h>
#include <fcntl.h>
#include <cstring>
#include "MyScheduler.h"
#include "MySemaphore.h"

using namespace std;

MyScheduler sched;
MySemaphore sema("Screen Sema");

WINDOW *create_window(int height, int width, int starty, int startx);
void write_window(WINDOW *win, const char *text);
void write_window(WINDOW *win, int x, int y, const char *text);
void display_help(WINDOW *win);
void* run_task_window(void *args);
void* run_console_window(void *args);

int main() {
  sema.set_sched_ptr(&sched);
  pthread_t t1, t2, t3, console_t;


  initscr();
  int y, x;
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  wprintw(stdscr, "Screen height = %d, Screen width = %d\n", max_y, max_x);
  getyx(stdscr, y, x);
  wprintw(stdscr, "Currecnt y = %d, Current x = %d\n", y, x);
  sleep(1);
  refresh();

  WINDOW *heading_win = newwin(12, 80, 3, 2);
  box(heading_win, 0, 0);
  mvwprintw(heading_win, 1, 2, "ULTIMA 2.0 (Spring 2019)");
  mvwprintw(heading_win, 2, 2, "Starting Thread 1.....");
  mvwprintw(heading_win, 3, 2, "Starting Thread 2.....");
  mvwprintw(heading_win, 4, 2, "Starting Thread 3.....");
  mvwprintw(heading_win, 5, 2, "Press ctrl-c to exit the program....");
  wrefresh(heading_win);

  WINDOW *t1_win = create_window(15, 25, 15, 2);
  WINDOW *t2_win = create_window(15, 25, 15, 30);
  WINDOW *t3_win = create_window(15, 25, 15, 57);
  WINDOW *log_win = create_window(10, 60, 30, 2);
  WINDOW *console_win = create_window(10, 20, 30, 62);
  WINDOW *d_window = create_window(37, 60, 3, 82);
  write_window(d_window, 1, 1, "........dump() info........");  
  write_window(log_win, 1, 5, "....log....\n");
  write_window(console_win, 1, 1, "....console....\n");
  write_window(console_win, 2, 1, "ULTIMA #");

  sema.set_dump_win(d_window);
  sched.setLogWindow(log_win);

  cbreak();
  noecho();

  sched.create_task(&t1, run_task_window, t1_win, "Thread 1", MyScheduler::RUNNING);
  sleep(1);
  sched.create_task(&t2, run_task_window, t2_win, "Thread 2", MyScheduler::RUNNING);
  sleep(1);
  sched.create_task(&t3, run_task_window, t3_win, "Thread 3", MyScheduler::RUNNING);
  sleep(1);
  sched.create_task(&console_t, run_console_window, console_win, "Console", MyScheduler::RUNNING);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(console_t, NULL);

  endwin();

  return(0);
}

WINDOW *create_window(int height, int width, int starty, int startx) {
  WINDOW *win = newwin(height, width, starty, startx);
  scrollok(win, true);
  scroll(win);
  box(win, 0, 0);
  wrefresh(win);
  return win;
}

void write_window(WINDOW *win, const char *text) {
  wprintw(win, text);
  box(win, 0, 0);
  wrefresh(win);
}

void write_window(WINDOW *win, int y, int x, const char *text) {
  mvwprintw(win, y, x, text);
  box(win, 0, 0);
  wrefresh(win);
}

void display_help(WINDOW *win) {
  wclear(win);
  write_window(win, 1, 1, "...help...");
  write_window(win, 2, 1, "1= run 1");
  write_window(win, 3, 1, "2= run 2");
  write_window(win, 4, 1, "3= run 3");
  write_window(win, 5, 1, "c= clear screen");
  write_window(win, 6, 1, "h= help screen");
  write_window(win, 7, 1, "d= dump");
}

void* run_task_window(void *args) {
  MyScheduler::TCB* process = (MyScheduler::TCB*)args;
  int tid = process->tid;
  char name[64];
  strcpy(name, process->tname);
  WINDOW* win = process->win_ptr;
  WINDOW* log_win = process->log_win_ptr;

  char buff[256];
  sprintf(buff, " Starting %s....\n", name);

  sema.down(tid);
  write_window(win, buff);
  sema.up();

  int CPU_Quantum = 0;

  while (true) {
    if (process->state == MyScheduler::RUNNING) {
      sprintf(buff, " Task %d running #%d\n", tid, CPU_Quantum++);

      sema.down(tid);
      write_window(win, buff);
      write_window(log_win, buff);
      sema.up();

      sched.run_next(tid);
    }
    else if (process->state == MyScheduler::DEAD)
      break;
    else
      pthread_yield();
  }
}

void* run_console_window(void *args) {
  MyScheduler::TCB* process = (MyScheduler::TCB*)args;
  int tid = process->tid;
  char name[64];
  strcpy(name, process->tname);
  WINDOW* win = process->win_ptr;
  WINDOW* log_win = process->log_win_ptr;

  nodelay(win, true);
  char buf[256];
  int input = ERR;

  while (true) {
    if (process->state == MyScheduler::RUNNING) {
      
      sema.down(tid);

      input = wgetch(win);
      
	switch (input) {
	case '1':
	case '2':
	case '3':
	  sprintf(buf, "%c\n", input);
	  write_window(win, buf);
	  
	  sprintf(buf, " You typed = %c\n", input);
	  write_window(win, buf);
	  
	  write_window(log_win, buf);
	  
	  break;
	case 'c':
	  wclear(win);
	  write_window(win, 1, 1, "ULTIMA #");
	  break;
	case 'h':
	  display_help(win);
	  write_window(win, 8, 1, "ULTIMA #");
	  break;
	case 'd':
	  sema.dump(4);
	  wclear(win);
	  write_window(win, 1, 1, "ULTIMA #\n press R to resume");
	  nodelay(win, false);
	  do {
	    input = wgetch(win);
	  } while (input != 'r');
	  nodelay(win, true);
	  sema.un_dump();
	  break;
	case ERR:
	  break;
	default:
	  sprintf(buf, " %c\n", input);
	  write_window(win, buf);
	  write_window(win, " -Invalid command\n");
	  write_window(log_win, buf);
	  write_window(log_win, " -Invalid command\n");
	  write_window(win, " ULTIMA #");
	  break;
	}
	
	sema.up();
	
	  sched.run_next(tid);
    }
    else if (process->state == MyScheduler::DEAD)
      break;
    else
      pthread_yield();
  }
}
