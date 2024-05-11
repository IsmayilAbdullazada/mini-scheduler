CC=gcc
CFLAGS=-I.

all: workload test_workload

workload: workload.o
	$(CC) -o workload workload.o

workload.o: workload.c workload.h
	$(CC) -c workload.c

test_workload: test_workload.o workload.o
	$(CC) -o test_workload test_workload.o workload.o

test_workload.o: test_workload.c workload.h
	$(CC) -c test_workload.c

test: test_workload
	valgrind --leak-check=full ./test_workload

clean:
	rm -f *.o workload test_workload