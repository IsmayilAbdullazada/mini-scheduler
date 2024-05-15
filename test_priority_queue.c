#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"
#include "priority_queue.h"

// Function to create a workload item for testing
workload_item* create_test_workload_item(int pid, int ts, int tf, int prio, int status, char* name) {
    workload_item *item = create_workload_item(pid, ts, tf, prio, status, name, strlen(name));
    return item;
}

int main() {
    // Test priority queue
    printf("Testing priority queue...\n");

    // Initialize priority queue
    priority_queue *pq = init_priority_queue(10);

    // Insert workload items
    insert(pq, create_test_workload_item(1, 0, 10, 5, 0, "Process A"));
    insert(pq, create_test_workload_item(2, 5, 15, 3, 0, "Process B"));
    insert(pq, create_test_workload_item(3, 10, 20, 7, 0, "Process C"));

    // Display priority queue
    display_priority_queue(pq);

    // Extract max
    workload_item *max_item = extract_max(pq);
    printf("Extracted max item: (prio: %d, pid: %d)\n", max_item->prio, max_item->pid);

    // Display priority queue after extraction
    display_priority_queue(pq);

    // Free priority queue
    free_priority_queue(pq);

    printf("Priority queue test complete.\n");

    return 0;
}