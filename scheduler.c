#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct scheduler {
    priority_queue *running_queue;  /** Pointer to the priority queue of running processes. */
    priority_queue *pending_queue;  /** Pointer to the priority queue of pending processes. */
    int cpu_capacity;               /** CPU capacity for scheduling processes. */
    int cpu_occupation;             /** Current total CPU occupation by running processes. */
};

scheduler* init_scheduler(workload_item **workloads, size_t num_events, int cpu_capacity) {
    scheduler* s = malloc(sizeof(scheduler));
    if (s == NULL) {
        printf("Error: Could not allocate memory for the scheduler\n");
        exit(1);
    }

    // Create empty running queue with number of events
    s->running_queue = init_priority_queue(num_events);
    // Create running queue with items from workloads array
    s->pending_queue = build_priority_queue(workloads, num_events);
    s->cpu_capacity = cpu_capacity;
    s->cpu_occupation = 0;

    if (s->running_queue == NULL || s->pending_queue == NULL) {
        printf("Error: Could not allocate memory for the priority queues\n");
        exit(1);
    }

    return s;
}

void clean_up_scheduler(scheduler* s) {
    if (s) {
      if (s->pending_queue) free_priority_queue(s->pending_queue);
      if (s->running_queue) free_priority_queue(s->running_queue);
      free(s);
    }
}

/**
 * @brief Increases the idle time and finish time of a process.
 * 
 * @param process Pointer to the workload item representing the process.
 */
void increase_idle_tf(workload_item *process) {
  set_idle(process, get_idle(process) + 1);
  set_tf(process, get_tf(process) + 1);
}

/**
 * @brief Checks if it is possible for a process to execute at a given time.
 * 
 * @param time The current time.
 * @param process Pointer to the workload item representing the process.
 * @return int Returns 1 if finish time of the process >= than the current time; otherwise, returns 0.
 */
int is_possible_process(int time, workload_item *process) {
    return time <= get_tf(process);
}

/**
 * @brief Checks if a process is the current process at a given time.
 * 
 * @param time The current time.
 * @param process Pointer to the workload item representing the process.
 * @return int Returns 1 if the current time is between start and finish time of the process; otherwise, returns 0.
 */
int is_current_process(int time, workload_item *process) {
    return (get_ts(process) <= time && time <= get_tf(process));
}

/**
 * @brief Schedules a process to run by extracting max element from the pending queue and adding to the running queue
 * 
 * @param s Pointer to the scheduler.
 * @param process Pointer to the workload item representing the process.
 * @param current_time The current time.
 */
void schedule(scheduler *s, workload_item *process, int current_time) {
  if (process == NULL) return;
  insert(s->running_queue, process, current_time);
  extract_max(s->pending_queue, current_time);
  s->cpu_occupation += get_priority(process);
}

/**
 * @brief Deschedules a process by removing from the running queue and adding back to the pending queue
 * 
 * @param s Pointer to the scheduler.
 * @param process Pointer to the workload item representing the process.
 * @param current_time The current time.
 */
void deschedule(scheduler* s, workload_item *process, int current_time) {
  if (process == NULL) return;
  insert(s->pending_queue, process, current_time);
  delete(s->running_queue, process, current_time);
  s->cpu_occupation -= get_priority(process);
}

void schedule_processes(scheduler* s, int N) {
  // Open trace file to write what'happening in the algorithm on
  FILE *trace_file = fopen("trace.txt", "w");
  if (trace_file == NULL) {
      printf("Error: Could not open trace file for writing\n");
      return;
  }

  // Iterate over each timestep from 0 to N inclusive.
  for (int timestep = 0; timestep <= N; timestep++) {
    fprintf(trace_file, "[t=%d]\n", timestep);

    // Process running queue first, remove finished processes
    int index_rq = 0;
    while (index_rq < get_size(s->running_queue)) {
      workload_item *running_process = get_heap(s->running_queue)[index_rq++];
      if (!is_possible_process(timestep, running_process)) {
        // If a process has finished, write to the trace and remove it from the running queue
        fprintf(trace_file, "process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(running_process), get_priority(running_process), get_cmd(running_process), timestep-1);
        delete(s->running_queue, running_process, timestep);
        // Update CPU occupation
        s->cpu_occupation -= get_priority(running_process);
        fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", s->cpu_occupation);
        // Restart the loop to handle any additional finished processes
        index_rq = 0;
      }
    }

    // Process pending queue
    int index_pq = 0;
    while (index_pq < get_size(s->pending_queue)) {
      workload_item *pending_process = get_heap(s->pending_queue)[index_pq++];
      int in_pq = true;
      int pq_changed = false;

      // Check if the process can execute and its priority is within CPU capacity
      if (is_current_process(timestep, pending_process) && get_priority(pending_process) <= s->cpu_capacity) {
        // Ensure that the total priority of running processes does not exceed the CPU capacity
        while ((min_running_process + get_priority(pending_process)) > s->cpu_capacity) {
          workload_item *min_running_process = get_min(s->running_queue);
          if (get_priority(pending_process) <= get_priority(min_running_process)){
            // If the process cannot fit due to priority, write to the trace and skip scheduling it
            fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: None, as min prio: pid=%d prio=%d ('%s') has greater priority\n", get_pid(pending_process), get_priority(pending_process), get_cmd(pending_process), get_pid(min_running_process), get_priority(min_running_process), get_cmd(min_running_process));
            fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", min_running_process);
            break;
          }
          // If another process with lower priority can be put to sleep, deschedule it
          fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: pid=%d prio=%d ('%s')\n", get_pid(pending_process), get_priority(pending_process), get_cmd(pending_process), get_pid(min_running_process), get_priority(min_running_process), get_cmd(min_running_process));
          deschedule(s, min_running_process, timestep);
          pq_changed = true;
          fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", min_running_process);
        }
        // If the process can fit, schedule it and update the trace
        if (s->cpu_occupation + get_priority(pending_process) <= s->cpu_capacity) {
          schedule(s, pending_process, timestep);
          in_pq = false;
          fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... added to running queue\n", get_pid(pending_process), get_priority(pending_process), get_cmd(pending_process));
          fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", min_running_process);
          pq_changed = true;
        }
        // If the process is still current, increase its idle time and finish time
        if (is_current_process(timestep, pending_process) && in_pq)
          increase_idle_tf(pending_process);
      }
      // Restart the loop if the priority queue has been changed
      if (pq_changed) index_pq = 0;
    }

    // Display the state of running and pending queues in the trace
    char *running_queue_str = display_priority_queue(s->running_queue, 0);
    char *pending_queue_str = display_priority_queue(s->pending_queue, 0);

    fprintf(trace_file, "running: %s\n", running_queue_str);
    fprintf(trace_file, "pending: %s\n\n", pending_queue_str);

    free(running_queue_str);
    free(pending_queue_str);
  }
  fclose(trace_file);
}
