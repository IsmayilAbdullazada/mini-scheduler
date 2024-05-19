#define MAX_EVENTS 100

/**
 * @brief Represents a priority queue.
 */
typedef struct priority_queue_t priority_queue;

/**
 * @brief Initializes a priority queue with the given capacity.
 * 
 * @param capacity The maximum number of elements the queue can hold.
 * @return A pointer to the initialized priority queue.
 */
priority_queue *init_priority_queue(size_t capacity);

/**
 * @brief Builds a priority queue from an array of workload items.
 * 
 * @param workloads An array of workload items.
 * @param num_events The number of events in the array.
 * @return A pointer to the built priority queue.
 */
priority_queue* build_priority_queue(workload_item **workloads, size_t num_events);

/**
 * @brief Inserts a workload item into the priority queue.
 * 
 * @param pq The priority queue.
 * @param process The workload item to insert.
 * @param current_time The current time.
 */
void insert(priority_queue *pq, workload_item *process, int current_time);

/**
 * @brief Deletes a workload item from the priority queue.
 * 
 * @param pq The priority queue.
 * @param process The workload item to delete.
 * @param current_time The current time.
 */
void delete(priority_queue *pq, workload_item *process, int current_time);

/**
 * @brief Extracts the maximum workload item from the priority queue.
 * 
 * @param pq The priority queue.
 * @param current_time The current time.
 * @return The maximum workload item.
 */
workload_item* extract_max(priority_queue *pq, int current_time);

/**
 * @brief Checks if the priority queue is empty.
 * 
 * @param pq The priority queue.
 * @return 1 if the priority queue is empty, otherwise 0.
 */
int is_empty(priority_queue *pq);

/**
 * @brief Frees the memory allocated for the priority queue.
 * 
 * @param pq The priority queue to free.
 */
void free_priority_queue(priority_queue *pq);

/**
 * @brief Displays the contents of the priority queue.
 * 
 * @param pq The priority queue.
 * @param print If true, the contents will be printed to stdout.
 * @return A string representation of the priority queue.
 */
char* display_priority_queue(priority_queue *pq, int print);

/**
 * @brief Gets the current size of the priority queue.
 * 
 * @param pq The priority queue.
 * @return The current size of the priority queue.
 */
size_t get_size(const priority_queue *pq);

/**
 * @brief Gets the binary heap array of the priority queue.
 * 
 * @param pq The priority queue.
 * @return The binary heap array of the priority queue.
 */
workload_item** get_heap(const priority_queue *pq);

/**
 * @brief Gets the maximum workload item from the priority queue.
 * 
 * @param pq The priority queue.
 * @return The maximum workload item.
 */
workload_item* get_max(const priority_queue *pq);

/**
 * @brief Gets the minimum workload item from the priority queue.
 * 
 * @param pq The priority queue.
 * @return The minimum workload item.
 */
workload_item* get_min(const priority_queue *pq);

/**
 * @brief Checks if workload item 'a' has higher priority than workload item 'b'.
 * 
 * @param a The first workload item.
 * @param b The second workload item.
 * @param current_time The current time.
 * @return 1 if 'a' has higher priority than 'b', otherwise 0.
 */
int is_higher_priority(workload_item* a, workload_item* b, int current_time);

/**
 * @brief Swaps two workload item pointers.
 * 
 * @param a Pointer to the first workload item.
 * @param b Pointer to the second workload item.
 */
void swap(workload_item **a, workload_item **b);