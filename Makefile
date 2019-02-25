phase1.out: lab2_p2.o MySemaphore.o MyScheduler.o
	g++ -o phase1.out lab2_p2.o MySemaphore.o MyScheduler.o -lpthread -lncurses

lab2_p2.o: lab2_p2.cpp
	g++ -c lab2_p2.cpp

MyScheduler.o: MyScheduler.cpp MyScheduler.h
	g++ -c MyScheduler.cpp MyScheduler.h

MySemaphore.o: MySemaphore.cpp MySemaphore.h
	g++ -c MySemaphore.cpp MySemaphore.h -std=c++11
