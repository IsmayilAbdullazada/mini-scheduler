#include "workload.h"
#include "scheduler_priority_queue.h"

/**
 * @brief Struct representing a scheduler.
 * 
 */
typedef struct scheduler scheduler;

/**
 * @brief Initializes a scheduler with the given workload items, number of events, and CPU capacity.
 * 
 * @param workloads Array of workload items.
 * @param num_events Number of events.
 * @param cpu_capacity CPU capacity.
 * @return scheduler* Pointer to the initialized scheduler.
 */
scheduler* init_scheduler(workload_item **workloads, size_t num_events, int cpu_capacity);

/**
 * @brief Schedules processes based on the scheduler's policies and writes the trace to a file named "trace.txt".
 * 
 * @param s Pointer to the scheduler.
 * @param N Number of time steps.
 */
void schedule_processes(scheduler* scheduler, int N);

/**
 * @brief Cleans up memory allocated for the scheduler.
 * 
 * @param s Pointer to the scheduler to be cleaned up.
 */
void clean_up_scheduler(scheduler* scheduler);