#include <stdlib.h>
#include <limits.h>
#include "workload.h"
#include "priority_queue.h"

struct priority_queue_t {
    workload_item** heap;
    size_t capacity;
    size_t size;
};

workload_item** get_heap(const priority_queue *pq) {
    return pq->heap;
}

size_t get_size(const priority_queue *pq) {
    return pq->size;
}

priority_queue* init_priority_queue(size_t capacity) {
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (workload_item **)malloc(capacity * sizeof(workload_item*));
    return pq;
}

void heapify_down(priority_queue *pq, size_t index) {
    size_t left_child = 2 * index + 1;
    size_t right_child = 2 * index + 2;
    size_t largest = index;

    if (left_child < pq->size && is_higher_priority(pq->heap[left_child], pq->heap[largest])) {
        largest = left_child;
    }

    if (right_child < pq->size && is_higher_priority(pq->heap[right_child], pq->heap[largest])) {
        largest = right_child;
    }

    if (largest != index) {
        swap(&pq->heap[index], &pq->heap[largest]);
        heapify_down(pq, largest);
    }
}

void heapify_up(priority_queue *pq, size_t index) {
    if (index == 0) {
        return;
    }

    size_t parent = (index - 1) / 2;
    if (is_higher_priority(pq->heap[index], pq->heap[parent])) {
        swap(&pq->heap[parent], &pq->heap[index]);
        heapify_up(pq, parent);
    }
}

int is_higher_priority(workload_item* a, workload_item* b) {
    if (get_ts(a) <= get_ts(b) || (get_ts(a) == get_ts(b) && get_priority(a) >= get_priority(b))) {
        return 1;
    }
    return 0;
}

priority_queue* build_priority_queue(workload_item **workloads, size_t num_events) {
    priority_queue *pq = init_priority_queue(num_events);
    int i;
	for (i = 0; i < num_events; i++) {
		pq->heap[i] = workloads[i];
	}
	pq->size = i;
	for (i = (pq->size - 1) / 2; i>=0; i--) {
		heapify_down(pq, i);
	}
	return pq;
}

void insert(priority_queue *pq, workload_item *process) {
    if (pq->size == pq->capacity) {
        printf("Priority queue is full!\n");
        return;
    }

    pq->heap[pq->size] = process;
    pq->size++;
    heapify_up(pq, pq->size - 1);
}

void delete(priority_queue *pq, workload_item *process) {
  size_t index;
  for (index = 0; index < pq->size; index++) {
    if (get_pid(pq->heap[index]) == get_pid(process))
      break;
  }
  pq->heap[index] = pq->heap[pq->size - 1];
  pq->size--;
  heapify_down(pq, index);
}

workload_item* extract_max(priority_queue *pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty!\n");
        workload_item *null_process = create_workload_item(-1, -1, 0, 0, 0, NULL, -1); // {-1, -1, 0, 0, 0, NULL, -1}; // Return null process
        return null_process;
    }

    workload_item *max_process = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return max_process;
}

workload_item* get_max(const priority_queue *pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty!\n");
        return NULL;
    }

    return pq->heap[0];
}

workload_item* get_min(const priority_queue *pq) {
    if (pq->size == 0) {
        return NULL;
    }

    workload_item *min_process = NULL;
    int min_priority = INT_MAX;


    for (size_t i = 1; i < pq->size; ++i) {
        int current_priority = get_priority(pq->heap[i]);
        size_t current_ts = get_ts(pq->heap[i]);

        if (current_priority < min_priority){
            min_priority = current_priority;
            min_process = pq->heap[i];
        }
    }
    return min_process;
}

int is_empty(priority_queue *pq) {
    return pq->size == 0;
}

void free_priority_queue(priority_queue *pq) {
    if (pq) {
        if (pq->heap) {
            free(pq->heap);
        }
        free(pq);
    }
}

char* display_priority_queue(priority_queue *pq, int print) {
    size_t total_size = 3; // For the opening "[" and closing "]", and null terminator
    for (size_t i = 0; i < pq->size; i++) {
        total_size += snprintf(NULL, 0, "(%d, %d) ", get_priority(pq->heap[i]), get_pid(pq->heap[i]));
    }

    char *result = malloc(total_size);
    if (result == NULL) {
        printf("Error: Could not allocate memory for the queue display string\n");
        exit(1);
    }

    char *ptr = result;
    ptr += sprintf(ptr, "[");
    for (size_t i = 0; i < pq->size; i++) {
        ptr += sprintf(ptr, "(%d, %d) ", get_priority(pq->heap[i]), get_pid(pq->heap[i]));
    }
    sprintf(ptr, "]");

    if (print)
        printf("%s\n", result);

    return result;
}

void swap(workload_item **a, workload_item **b) {
    workload_item* temp = *a;
    *a = *b;
    *b = temp;
}
