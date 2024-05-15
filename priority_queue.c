#include <stdlib.h>
#include "workload.h"
#include "priority_queue.h"

// Structure to represent a priority queue
struct priority_queue_t {
    workload_item *heap;
    size_t capacity;
    size_t size;
};

size_t get_size(const priority_queue *pq) {
    return pq->size;
}

// Function to initialize a priority queue
priority_queue *init_priority_queue(size_t capacity) {
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (workload_item *)malloc(capacity * sizeof(workload_item));
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

// Function to compare two processes based on priority and ts
int is_higher_priority(workload_item a, workload_item b) {
    if (a.prio > b.prio || (a.prio == b.prio && a.ts < b.ts)) {
        return 1;
    }
    return 0;
}


// Function to insert a process into the priority queue
void insert(priority_queue *pq, workload_item *process) {
    if (pq->size == pq->capacity) {
        printf("Priority queue is full!\n");
        return;
    }

    pq->heap[pq->size] = *process;
    pq->size++;
    heapify_up(pq, pq->size - 1);
}

void delete(priority_queue *pq, workload_item *process) {
  size_t i;
  for (i = 0; i < pq->size; i++) {
    if (pq->heap[i].pid == process->pid) {
      break;
    }
  }
  if (i == pq->size) {
    printf("Element with pid %d not found in the queue\n", process->pid);
    return;
  }
  swap(&pq->heap[i], &pq->heap[pq->size - 1]);
  pq->size--;
  heapify_down(pq, i);
}

// Function to extract the highest priority process from the priority queue
workload_item* extract_max(priority_queue *pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty!\n");
        workload_item *null_process = create_workload_item(-1, -1, 0, 0, 0, NULL, -1); // {-1, -1, 0, 0, 0, NULL, -1}; // Return null process
        return null_process;
    }

    workload_item *max_process = &(pq->heap[0]);
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return max_process;
}

// Function to check if the priority queue is empty
int is_empty(priority_queue *pq) {
    return pq->size == 0;
}

// Function to free the memory allocated for the priority queue
void free_priority_queue(priority_queue *pq) {
    free(pq->heap);
    free(pq);
}

void display_priority_queue(priority_queue *pq) {
    for (size_t i = 0; i < pq->size; i++) {
        printf("(prio: %d, prid: %d), ", pq->heap[i].prio, pq->heap[i].pid);
    }
    printf("\n");	
}

void swap(workload_item *a, workload_item *b) {
    workload_item temp = *a;
    *a = *b;
    *b = temp;
}