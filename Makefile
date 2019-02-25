test.out: test.o MyScheduler.o
	g++ -o test.out test.o MyScheduler.o -lpthread -lncurses

test.o: test.cpp
	g++ -c test.cpp

MySemaphore.o: MySemaphore.cpp MySemaphore.h
	g++ -c MySemaphore.cpp MySemaphore.h 
	
