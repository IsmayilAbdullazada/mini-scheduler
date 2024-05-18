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

void clean_up_scheduler(scheduler* s) {
    if (s) {
      if (s->pending_queue)
        free_priority_queue(s->pending_queue);
      if (s->running_queue)
        free_priority_queue(s->running_queue);
      free(s);
    }
}

void increase_idle_tf(workload_item *process) {
  set_idle(process, get_idle(process)+1);
  set_tf(process, get_tf(process)+1);
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

void schedule(scheduler *s, workload_item *process) {
  if (process == NULL) return;
  insert(s->running_queue, process);
  delete(s->pending_queue, process);
}

void deschedule(scheduler* s, workload_item *process) {
  if (process == NULL) return;
  insert(s->pending_queue, process);
  delete(s->running_queue, process);
}

void schedule_processes(scheduler* s, int N) {
  FILE *trace_file = fopen("trace.txt", "w");
  if (trace_file == NULL) {
      printf("Error: Could not open trace file for writing\n");
      return;
  }

  for (int timestep = 0; timestep <= N; timestep++) {
    fprintf(trace_file, "[t=%d]\n", timestep);

    int index_rq = 0;
    while (index_rq < get_size(s->running_queue)) {
      workload_item *process = get_heap(s->running_queue)[index_rq];
      index_rq++;
      if (!is_possible_process(timestep, process)) {
        fprintf(trace_file, "process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(process), get_priority(process), get_cmd(process), timestep-1);
        delete(s->running_queue, process);
        fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
        index_rq = 0;
      }
    }

    // Process pending queue
    int index_pq = 0;
    while (index_pq < get_size(s->pending_queue)) {
      workload_item *process = get_heap(s->pending_queue)[index_pq];
      index_pq++;
      int is_in_pq = 1;
      int is_pq_change = 0;

      if (is_current_process(timestep, process) && get_priority(process) <= s->cpu_capacity) {
        while ((sum_priorities(s->running_queue) + get_priority(process)) > 20) {
          workload_item *min_process = get_min(s->running_queue);
          if (get_priority(process) <= get_priority(min_process)){
            fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: None, as min prio: pid=%d prio=%d ('%s') has greater priority\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process));
            fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
            break;
          }
          fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: pid=%d prio=%d ('%s')\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process));
          deschedule(s, min_process);
          is_pq_change = 1;
          fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
        }
        if (sum_priorities(s->running_queue) + get_priority(process) <= 20) {
          schedule(s, process);
          is_in_pq = 0;
          fprintf(trace_file, "schedule pid=%d prio=%d ('%s') ... added to running queue\n", get_pid(process), get_priority(process), get_cmd(process));
          fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", sum_priorities(s->running_queue));
          is_pq_change = 1;
        }
        if (is_current_process(timestep, process) && is_in_pq == 1)
          increase_idle_tf(process);
      }
      if (is_pq_change) index_pq = 0;
    }

    char *running_queue_str = display_priority_queue(s->running_queue, 0);
    char *pending_queue_str = display_priority_queue(s->pending_queue, 0);

    fprintf(trace_file, "running: %s\n", running_queue_str);
    fprintf(trace_file, "pending: %s\n\n", pending_queue_str);

    free(running_queue_str);
    free(pending_queue_str);
  }
  fclose(trace_file);
}