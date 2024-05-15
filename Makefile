CC=gcc
CFLAGS=-I.

all: test_workload test_priority_queue main

workload.o: workload.c workload.h
	$(CC) -c workload.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) -c priority_queue.c

test_workload.o: test_workload.c workload.h
	$(CC) -c test_workload.c

test_workload: test_workload.o workload.o
	$(CC) -o test_workload test_workload.o workload.o

main.o: main.c workload.h priority_queue.h
	$(CC) -c main.c

main: main.o workload.o priority_queue.o
	$(CC) -o main main.o workload.o priority_queue.o

main_valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all ./main

testw: test_workload
	valgrind --leak-check=full --show-leak-kinds=all ./test_workload

test_priority_queue.o: test_priority_queue.c priority_queue.h workload.h
	$(CC) -c test_priority_queue.c

test_priority_queue: test_priority_queue.o workload.o priority_queue.o
	$(CC) -o test_priority_queue test_priority_queue.o workload.o priority_queue.o

testpq: test_priority_queue
	valgrind --leak-check=full --show-leak-kinds=all ./test_priority_queue

clean:
	rm -f *.o workload test_workload main test_priority_queue
