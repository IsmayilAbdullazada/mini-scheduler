#define MAX_EVENTS 100

typedef struct priority_queue_t priority_queue;

priority_queue *init_priority_queue(size_t capacity);
priority_queue* build_priority_queue(workload_item **workloads, size_t num_events);

void insert(priority_queue *pq, workload_item *process, int current_time);
void delete(priority_queue *pq, workload_item *process, int current_time);

workload_item* extract_max(priority_queue *pq, int current_time);

int is_empty(priority_queue *pq);

void free_priority_queue(priority_queue *pq);

char* display_priority_queue(priority_queue *pq, int print);

size_t get_size(const priority_queue *pq);
workload_item** get_heap(const priority_queue *pq);

workload_item* get_max(const priority_queue *pq);
workload_item* get_min(const priority_queue *pq);

int is_higher_priority(workload_item* a, workload_item* b, int current_time);

void swap(workload_item **a, workload_item **b);