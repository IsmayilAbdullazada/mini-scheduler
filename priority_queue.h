#define MAX_EVENTS 100

typedef struct priority_queue_t priority_queue;

priority_queue *init_priority_queue(size_t capacity);

void insert(priority_queue *pq, workload_item *process);

workload_item* extract_max(priority_queue *pq);

int is_empty(priority_queue *pq);

void free_priority_queue(priority_queue *pq);

void display_priority_queue(priority_queue *pq);

size_t get_size(const priority_queue *pq);
workload_item* get_heap(const priority_queue *pq);

workload_item* get_max(const priority_queue *pq);
workload_item* get_min(const priority_queue *pq);

void delete(priority_queue *pq, workload_item *process);

void swap(workload_item *a, workload_item *b);

int is_higher_priority(workload_item a, workload_item b);