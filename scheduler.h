#include "workload.h"
#include "priority_queue.h"

typedef struct scheduler scheduler;

scheduler* init_scheduler(workload_item **workloads, size_t num_events, int cpu_capacity);
void schedule_processes(scheduler* scheduler, int N);
void clean_up_scheduler(scheduler* scheduler);