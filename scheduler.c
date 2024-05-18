#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"


struct scheduler {
    priority_queue *running_queue;
    priority_queue *pending_queue;
    int cpu_capacity;
};

scheduler* init_scheduler(workload_item **workloads, size_t num_events, int cpu_capacity) {
    scheduler* s = malloc(sizeof(scheduler));
    if (s == NULL) {
        printf("Error: Could not allocate memory for the scheduler\n");
        exit(1);
    }

    s->running_queue = init_priority_queue(num_events);
    s->pending_queue = build_priority_queue(workloads, num_events);
    s->cpu_capacity = cpu_capacity;

    if (s->running_queue == NULL || s->pending_queue == NULL) {
        printf("Error: Could not allocate memory for the priority queues\n");
        exit(1);
    }

    return s;
}

// Copy the rest of the functions from main.c to scheduler.c, replacing the global variables with the scheduler struct.

void clean_up_scheduler(scheduler* s) {
    while (!is_empty(s->pending_queue)) {
        workload_item *process = extract_max(s->pending_queue);
        free_workload_item(process);
    }

    while (!is_empty(s->running_queue)) {
        workload_item *process = extract_max(s->running_queue);
        free_workload_item(process);
    }

    free_priority_queue(s->pending_queue);
    free_priority_queue(s->running_queue);

    free(s);
}





int is_possible_process(int time, workload_item *process) {
    return time <= get_tf(process);
}

int is_current_process(int time, workload_item *process) {
    return (get_ts(process) <= time && time <= get_tf(process));
}

int sum_priorities(priority_queue *pq) {
  int sum = 0;
  for (int i = 0; i < get_size(pq); i++) {
    sum += get_priority(get_heap(pq)[i]);
  }
  return sum;
}

int deschedule(scheduler* s, workload_item *process) {
  if (process == NULL) return 0;
  insert(s->pending_queue, process);
  delete(s->running_queue, process);
  //printf("Before idle: %lu\n", get_idle(process));
  set_idle(process, get_idle(process)+1);
  //printf("After idle: %lu\n", get_idle(process));
  // printf("Before tf: %lu\n", get_tf(process));
  set_tf(process, get_tf(process)+1);
  // printf("After tf: %lu\n", get_tf(process));
  return 1;
}

void schedule_processes(scheduler* s, int N) {
  for (int timestep = 0; timestep <= N; timestep++) {
    printf("[t=%d]\n", timestep);

    int index_rq = 0;
    // for (int index_rq=0; index_rq < get_size(running_queue); ++index_rq) {
    while (index_rq < get_size(s->running_queue)) {
      // printf("index in rq: %d\n", index_rq);
      workload_item *process = get_heap(s->running_queue)[index_rq];
      index_rq++;
      // printf("index: %d\n", index_rq);
      // display_priority_queue(running_queue);
      // printf("\nrq size: %lu\n", get_size(running_queue));
      // printf("process pid=%d prio=%d tf=%lu ('%s')\n", process->pid, process->prio, process->tf, process->cmd);
      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(process), get_priority(process), get_cmd(process), timestep-1);
        delete(s->running_queue, process);
        printf("CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
        index_rq = 0;
      }
    }

    // Process pending queue
    int index_pq = 0;
    while (index_pq < get_size(s->pending_queue)) {
      workload_item *process = get_heap(s->pending_queue)[index_pq];
      index_pq++;
      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(process), get_priority(process), get_cmd(process), timestep-1);
        delete(s->pending_queue, process);
        continue;
      }

      if (is_current_process(timestep, process) && get_priority(process) <= s->cpu_capacity) {
        while ((sum_priorities(s->running_queue) + get_priority(process)) > 20) {
          workload_item *min_process = get_min(s->running_queue);
          if (get_priority(process) <= get_priority(min_process)){
            printf("schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: None, as min prio: pid=%d prio=%d ('%s') has greater priority\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process));
            printf("CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
            break;
          }
          printf("schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: pid=%d prio=%d ('%s')\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process)); 
          deschedule(s, min_process);
          printf("CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
        }
        if (sum_priorities(s->running_queue) + get_priority(process) <= 20) {
          insert(s->running_queue, process);
          printf("schedule pid=%d prio=%d ('%s') ... added to running queue\n", get_pid(process), get_priority(process), get_cmd(process));
          printf("CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
          delete(s->pending_queue, process);
          index_pq = 0;
        }
        // printf("index of process: %d\n", index_pq);
        // printf("After descheduling: \n");
        // printf("Pending queue: \n");
        // display_priority_queue(pending_queue);
        // printf("Running queue: \n");
        // display_priority_queue(running_queue);
        // printf("\n");
        //   printf("Timestep: %d,,, Ts: %lu", timestep, process->ts);
      }
    }

    printf("running: [");
    display_priority_queue(s->running_queue);
    printf("]\n");
    printf("pending: [");
    display_priority_queue(s->pending_queue);
    printf("]\n\n");
  }
}

// Function to print a horizontal line segment of a given length
void print_line(int length) {
  for (int i = 0; i < length; i++) {
    printf(".");
  }
  printf("\n");
}


