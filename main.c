#include <stdio.h>
#include <stdlib.h>

#define MAX_EVENTS 100

// Structure to represent a process
typedef struct workload_item_t {
    int pid;        //< the event id
    int ppid;       //< the event parent id
    size_t ts;      //< start date
    size_t tf;      //< finish date
    size_t idle;    //< total time the process has been idle;
    char *cmd;      //< the binary name
    int prio;       //< process priority
} workload_item;

// Structure to represent a priority queue
struct priority_queue_t {
    workload_item *heap;
    size_t capacity;
    size_t size;
};
typedef struct priority_queue_t priority_queue;

// Function to initialize a priority queue
priority_queue *init_priority_queue(size_t capacity) {
    priority_queue *pq = (priority_queue *)malloc(sizeof(priority_queue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (workload_item *)malloc(capacity * sizeof(workload_item));
    return pq;
}

// Function to swap two processes
void swap(workload_item *a, workload_item *b) {
    workload_item temp = *a;
    *a = *b;
    *b = temp;
}

// Function to compare two processes based on priority and ts
int is_higher_priority(workload_item a, workload_item b) {
    if (a.prio > b.prio || (a.prio == b.prio && a.ts < b.ts)) {
        return 1;
    }
    return 0;
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


// Function to insert a process into the priority queue
void insert(priority_queue *pq, workload_item process) {
    if (pq->size == pq->capacity) {
        printf("Priority queue is full!\n");
        return;
    }

    pq->heap[pq->size] = process;
    pq->size++;
    heapify_up(pq, pq->size - 1);
}

// Function to extract the highest priority process from the priority queue
workload_item extract_max(priority_queue *pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty!\n");
        workload_item null_process = {-1, -1, 0, 0, 0, NULL, -1}; // Return null process
        return null_process;
    }

    workload_item max_process = pq->heap[0];
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


#define CPU_CAPACITY 20



void display_priority_queue(priority_queue *pq) {
    for (size_t i = 0; i < pq->size; i++) {
        printf("(prio: %d, prid: %d), ", pq->heap[i].prio, pq->heap[i].pid);
    }
    printf("\n");	
}


priority_queue *running_queue;
priority_queue *pending_queue;

void set_up_scedulor(workload_item* workloads, size_t num_events){
    running_queue = init_priority_queue(num_events);
    pending_queue = init_priority_queue(num_events);

    if (running_queue == NULL || pending_queue == NULL) {
        printf("Error: Could not allocate memory for the priority queues\n");
        exit(1);
    }

    if (workloads == NULL) {
        printf("Error: No workloads provided\n");
        exit(1);
    }

    
    for (size_t i = 0; i < num_events; i++) {
   
        insert(pending_queue, workloads[i]);
        
    }
    
}

int is_current_process(int time, workload_item process) {
    return (process.ts <= time && time <= process.tf);
}

void schedule_processes() {
    static int current_time = 0;
    static int cpu_capacity = CPU_CAPACITY;

    
    // add processes to running queue
    while (!is_empty(pending_queue) ) {
        workload_item process = extract_max(pending_queue);

        if (is_current_process(current_time, process) &&  (cpu_capacity >= process.prio)) {
            insert(running_queue, process);
        } else {
            insert(pending_queue, process);
        }


        if (cpu_capacity == 0) {
            break;
        }
    }

    // remove completed processes


}

int main() {

    workload_item workloads[] = {
        {0, -1, 0, 18, 0, "init", 10},
        {1, 0, 1, 16, 0, "bash", 1},
        {2, 0, 3, 16, 0, "bash", 1},
        {3, 1, 4, 6, 0, "find", 2},
        {4, 1, 7, 9, 0, "gcc", 5},
        {5, 4, 8, 9, 0, "ld", 4},
        {6, 2, 10, 13, 0, "ssh", 3},
        {7, 6, 11, 13, 0, "crypt", 5},
        {8, 2, 14, 16, 0, "snake", 4},
        {9, 1, 14, 15, 0, "cat", 5}
    };




    size_t num_events = sizeof(workloads) / sizeof(workloads[0]);
    set_up_scedulor(workloads, num_events);

    printf("Pending Queue size: %d\n ", pending_queue->size);	
    display_priority_queue(pending_queue);
    display_priority_queue(running_queue);
    return 0;
}