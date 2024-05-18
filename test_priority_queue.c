#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "workload.h"
#include "priority_queue.h"

workload_item* create_test_workload_item(int pid, int ppid, size_t ts, size_t tf, size_t idle, char* cmd, int prio) {
    workload_item *item = create_workload_item(pid, ppid, ts, tf, idle, cmd, prio);
    return item;
}

int main() {
    printf("Testing priority queue...\n");

    priority_queue *pq = init_priority_queue(10);

    insert(pq, create_test_workload_item(1, 0, 0, 10, 0, "Process A", 5));
    insert(pq, create_test_workload_item(2, 0, 5, 15, 0, "Process B", 3));
    insert(pq, create_test_workload_item(3, 0, 10, 20, 0, "Process C", 7));
    insert(pq, create_test_workload_item(4, 0, 2, 12, 0, "Process D", 5));
    insert(pq, create_test_workload_item(5, 0, 8, 18, 0, "Process E", 2));
    insert(pq, create_test_workload_item(6, 0, 1, 11, 0, "Process F", 3));
    insert(pq, create_test_workload_item(7, 0, 6, 16, 0, "Process G", 7));
    insert(pq, create_test_workload_item(8, 0, 3, 13, 0, "Process H", 1));
    insert(pq, create_test_workload_item(9, 0, 9, 19, 0, "Process I", 4));
    insert(pq, create_test_workload_item(10, 0, 4, 14, 0, "Process J", 6));

    printf("Priority queue before deletion:\n");
    display_priority_queue(pq, 1);

    delete(pq, create_test_workload_item(1, 0, 0, 10, 0, "Process A", 5));
    delete(pq, create_test_workload_item(2, 0, 5, 15, 0, "Process B", 3));
    delete(pq, create_test_workload_item(3, 0, 10, 20, 0, "Process C", 7));
    delete(pq, create_test_workload_item(4, 0, 2, 12, 0, "Process D", 5));
    delete(pq, create_test_workload_item(5, 0, 8, 18, 0, "Process E", 2));
    delete(pq, create_test_workload_item(6, 0, 1, 11, 0, "Process F", 3));
    delete(pq, create_test_workload_item(7, 0, 6, 16, 0, "Process G", 7));
    delete(pq, create_test_workload_item(8, 0, 3, 13, 0, "Process H", 1));
    delete(pq, create_test_workload_item(9, 0, 9, 19, 0, "Process I", 4));
    delete(pq, create_test_workload_item(10, 0, 4, 14, 0, "Process J", 6));

    printf("Priority queue after deletion:\n");
    display_priority_queue(pq, 1);
    printf("Size after deletion: %lu\n", get_size(pq));
    free_priority_queue(pq);

    printf("Priority queue test complete.\n");

    return 0;
}
