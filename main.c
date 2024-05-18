#include <stdio.h>
#include <stdlib.h>
#include "workload.h"
#include "priority_queue.h"

#define CPU_CAPACITY 20

priority_queue *running_queue;
priority_queue *pending_queue;

void set_up_scheduler(workload_item **workloads, size_t num_events){
    if (workloads == NULL) {
        printf("Error: No workloads provided\n");
        exit(1);
    }
    running_queue = init_priority_queue(num_events);
  
    pending_queue = build_priority_queue(workloads, num_events);
    if (running_queue == NULL || pending_queue == NULL) {
        printf("Error: Could not allocate memory for the priority queues\n");
        exit(1);
    }

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

int deschedule(workload_item *process) {
  if (process == NULL) return 0;
  insert(pending_queue, process);
  delete(running_queue, process);
  //printf("Before idle: %lu\n", get_idle(process));
  set_idle(process, get_idle(process)+1);
  //printf("After idle: %lu\n", get_idle(process));
  // printf("Before tf: %lu\n", get_tf(process));
  set_tf(process, get_tf(process)+1);
  // printf("After tf: %lu\n", get_tf(process));
  return 1;
}

void schedule_processes(int N) {
  for (int timestep = 0; timestep <= N; timestep++) {
    printf("[t=%d]\n", timestep);

    int index_rq = 0;
    // for (int index_rq=0; index_rq < get_size(running_queue); ++index_rq) {
    while (index_rq < get_size(running_queue)) {
      // printf("index in rq: %d\n", index_rq);
      workload_item *process = get_heap(running_queue)[index_rq];
      index_rq++;
      // printf("index: %d\n", index_rq);
      // display_priority_queue(running_queue);
      // printf("\nrq size: %lu\n", get_size(running_queue));
      // printf("process pid=%d prio=%d tf=%lu ('%s')\n", process->pid, process->prio, process->tf, process->cmd);
      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(process), get_priority(process), get_cmd(process), timestep-1);
        delete(running_queue, process);
        printf("CPU occupation: CPU[0]=%d\n", sum_priorities(running_queue));
        index_rq = 0;
      }
    }

    // Process pending queue
    int index_pq = 0;
    while (index_pq < get_size(pending_queue)) {
      workload_item *process = get_heap(pending_queue)[index_pq];
      index_pq++;
      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", get_pid(process), get_priority(process), get_cmd(process), timestep-1);
        delete(pending_queue, process);
        continue;
      }

      if (is_current_process(timestep, process) && get_priority(process) <= CPU_CAPACITY) {
        while ((sum_priorities(running_queue) + get_priority(process)) > 20) {
          workload_item *min_process = get_min(running_queue);
          if (get_priority(process) <= get_priority(min_process)){
            printf("schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: None, as min prio: pid=%d prio=%d ('%s') has greater priority\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process));
            printf("CPU occupation: CPU[0]=%d\n", sum_priorities(running_queue));
            break;
          }
          printf("schedule pid=%d prio=%d ('%s') ... can't fit. Pick process to put asleep: pid=%d prio=%d ('%s')\n", get_pid(process), get_priority(process), get_cmd(process), get_pid(min_process), get_priority(min_process), get_cmd(min_process)); 
          deschedule(min_process);
          printf("CPU occupation: CPU[0]=%d\n", sum_priorities(running_queue));
        }
        if (sum_priorities(running_queue) + get_priority(process) <= 20) {
          insert(running_queue, process);
          printf("schedule pid=%d prio=%d ('%s') ... added to running queue\n", get_pid(process), get_priority(process), get_cmd(process));
          printf("CPU occupation: CPU[0]=%d\n", sum_priorities(running_queue));
          delete(pending_queue, process);
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
    display_priority_queue(running_queue);
    printf("]\n");
    printf("pending: [");
    display_priority_queue(pending_queue);
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

void print_chronogram(workload_item **workloads, size_t workload_size, int N) {
  // Print the top line
  int full_segments = N / 5;
  int remaining_chars = N % 5;
  // Print full segments
  printf("\t");
  for (int i = 0; i < full_segments; i++) {
      printf("|....");
  }

  // Print the remaining characters
  if (remaining_chars) {
    printf("|");
    for (int i = 0; i < remaining_chars; i++) {
        printf(".");
    }
  }
  printf("\n");

  // Print the process names and execution timelines
  for (size_t i = 0; i < workload_size; i++) {
    printf("%-8s", get_cmd(workloads[i]));
    for (int t = 0; t < get_tf(workloads[i]); t++) {
      if (get_ts(workloads[i]) <= t && t < get_ts(workloads[i]) + get_tf(workloads[i])) {
        printf("X");
      } else {
        printf(" ");
      }
    }
    printf("\t\t\t(tf=%lu,idle=%lu)\n", get_tf(workloads[i]), get_idle(workloads[i]));
  }
}

void clean_up_scheduler() {
    while (!is_empty(pending_queue)) {
        workload_item *process = extract_max(pending_queue);
        free_workload_item(process);
    }

    while (!is_empty(running_queue)) {
        workload_item *process = extract_max(running_queue);
        free_workload_item(process);
    }

    free_priority_queue(pending_queue);
    free_priority_queue(running_queue);
}

int main() {
    // workload_item workloads[] = {
    //     {0, -1, 0, 18, 0, "init", 10},
    //     {1, 0, 1, 16, 0, "bash", 1},
    //     {2, 0, 3, 16, 0, "bash", 1},
    //     {3, 1, 4, 6, 0, "find", 2},
    //     {4, 1, 7, 9, 0, "gcc", 5},
    //     {5, 4, 8, 9, 0, "ld", 4},
    //     {6, 2, 10, 13, 0, "ssh", 3},
    //     {7, 6, 11, 13, 0, "crypt", 5},
    //     {8, 2, 14, 16, 0, "snake", 4},
    //     {9, 1, 14, 15, 0, "cat", 5}
    // };
    
    // size_t num_events = sizeof(workloads) / sizeof(workloads[0]);

    workload_item **workloads = malloc(MAX_ITEMS * sizeof(workload_item *));

    int num_workloads = get_workload_items("workload_input.txt", workloads, MAX_ITEMS);

    if (num_workloads == -1) {
        printf("Error: Could not read workload items from file.\n");
        return 1;
    }

    /*    
    for (int i = 0; i < num_workloads; i++) {
        // Access each workload item through the array of pointers
        printf("Workload Item %d: pid=%d, ppid=%d, ts=%zu, tf=%zu, idle=%zu, cmd=%s, prio=%d\n",
               i, get_pid(workloads[i]), get_ppid(workloads[i]), get_ts(workloads[i]),
               get_tf(workloads[i]), get_idle(workloads[i]), get_cmd(workloads[i]), get_priority(workloads[i]));
    }
    */

    int N = 20;
    set_up_scheduler(workloads, num_workloads);
    print_chronogram(workloads, num_workloads, N);
    schedule_processes(N);
    print_chronogram(workloads, num_workloads, N);
    // clean_up_scheduler();
    /*
    printf("Pending Queue size: %lu\n ", get_size(pending_queue));	
    display_priority_queue(pending_queue);
    display_priority_queue(running_queue);
    */
    for (int i = 0; i < MAX_ITEMS; i++) {
        free_workload_item(workloads[i]);
    }
    free(workloads);

    free_priority_queue(pending_queue);
    free_priority_queue(running_queue);


    return 0;
}