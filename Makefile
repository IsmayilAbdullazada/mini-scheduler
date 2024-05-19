CC=gcc
CFLAGS=-I.

all: test_workload test_priority_queue main

scheduler.o: scheduler.c scheduler.h
	$(CC) -c scheduler.c

workload.o: workload.c workload.h
	$(CC) -c workload.c

priority_queue.o: scheduler_priority_queue.c scheduler_priority_queue.h
	$(CC) -c scheduler_priority_queue.c

test_workload.o: test_workload.c workload.h
	$(CC) -c test_workload.c

test_workload: test_workload.o workload.o
	$(CC) -o test_workload test_workload.o workload.o

main.o: main.c workload.h scheduler_priority_queue.h scheduler.h
	$(CC) -c main.c

main: main.o workload.o scheduler_priority_queue.o scheduler.o
	$(CC) -o sched main.o workload.o scheduler_priority_queue.o scheduler.o

main_valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all ./sched

test_workload_valgrind: test_workload
	valgrind --leak-check=full --show-leak-kinds=all ./test_workload

test_priority_queue.o: test_priority_queue.c scheduler_priority_queue.h workload.h
	$(CC) -c test_priority_queue.c

test_priority_queue: test_priority_queue.o workload.o scheduler_priority_queue.o
	$(CC) -o test_priority_queue test_priority_queue.o workload.o scheduler_priority_queue.o

test_priority_queue_valgrind: test_priority_queue
	valgrind --leak-check=full --show-leak-kinds=all ./test_priority_queue

clean:
	rm -f *.o *.exe workload test_workload sched test_priority_queue trace.txt
