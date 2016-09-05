all: p2

p2: main.o llist.o grabm.o
	g++ main.o llist.o grabm.o -o p2

main.o: main.cpp record.h llist.h grabm.h
	g++ -c main.cpp -I.

llist.o: llist.cpp record.h llist.h grabm.h
	g++ -c llist.cpp -I.

grabm.o: grabm.cpp record.h llist.h grabm.h
	g++ -c grabm.cpp -I.


