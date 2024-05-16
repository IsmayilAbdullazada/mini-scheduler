#include <stdio.h>
#include <stdlib.h>
#include "workload.h"
#include "priority_queue.h"


#define CPU_CAPACITY 20


priority_queue *running_queue;
priority_queue *pending_queue;

void set_up_scheduler(workload_item **workloads, size_t num_events){
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

int is_possible_process(int time, workload_item *process) {
    return time <= get_tf(process);
}

int is_current_process(int time, workload_item *process) {
    return (get_ts(process) <= time && time <= get_tf(process));
}

int sum_priorities(priority_queue *pq) {
  int sum = 0;
  for (int i = 0; i < get_size(pq); i++) {
    sum += get_heap(pq)[i].prio;
  }
  return sum;
}

int deschedule() {
  workload_item *min_process = get_min(running_queue);
  if (min_process == NULL) return 0;
  printf("Descheduling Pid %d %s\n", min_process->pid, min_process->cmd);
  insert(pending_queue, min_process);
  delete(running_queue, min_process);
  // printf("Before idle: %lu\n", get_idle(min_process));
  set_idle(min_process, get_idle(min_process)+1);
  // printf("After idle: %lu\n", get_idle(min_process));
  // printf("Before tf: %lu\n", get_tf(min_process));
  set_tf(min_process, get_tf(min_process)+1);
  // printf("After tf: %lu\n", get_tf(min_process));
  return 1;
}

void schedule_processes(int N) {
  for (int timestep = 0; timestep <= N; timestep++) {
    printf("[t=%d]\n", timestep);
    // Process pending queue
    for (int index_pq=0; index_pq < get_size(pending_queue); ++index_pq) {
      workload_item *process = get_heap(pending_queue) + index_pq;

      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", process->pid, process->prio, process->cmd, timestep-1);
        delete(pending_queue, process);
        continue;
      }

      if (is_current_process(timestep, process) && process->prio <= CPU_CAPACITY) {
        while ((sum_priorities(running_queue) + process->prio) > 20) {
          printf("(prio: %d, pid: %d) cannot be added %s\n", process->prio, process->pid, process->cmd); 
          if (!deschedule()) break;
        }
        insert(running_queue, process);
        printf("schedule pid=%d prio=%d ('%s') ... added to running queue\n", process->pid, process->prio, process->cmd);
        delete(pending_queue, process);
        // printf("After descheduling: \n");
        // printf("Pending queue: \n");
        // display_priority_queue(pending_queue);
        // printf("Running queue: \n");
        // display_priority_queue(running_queue);
        // printf("\n");
        //   printf("Timestep: %d,,, Ts: %lu", timestep, process->ts);
      }
    }

    for (int index_rq=0; index_rq < get_size(running_queue); ++index_rq) {
      workload_item *process = get_heap(running_queue) + index_rq;
      if (!is_possible_process(timestep, process)) {
        printf("process pid=%d prio=%d ('%s') finished after time t=%d\n", process->pid, process->prio, process->cmd, timestep-1);
        delete(running_queue, process);
        continue;
      }
    }

    printf("CPU occupation: CPU[0]=%d\n", sum_priorities(running_queue));
    printf("running: [");
    display_priority_queue(running_queue);
    printf(" ]\n");
    printf("pending: [");
    display_priority_queue(pending_queue);
    printf(" ]\n\n");

  }
}

// Function to print a horizontal line segment of a given length
void print_line(int length) {
  for (int i = 0; i < length; i++) {
    printf(".");
  }
  printf("\n");
}

void print_chronogram(workload_item *workload[], size_t workload_size, int C) {
  int max_tf = 0;  // Maximum planned finish time among all processes

  // Find the maximum planned finish time
  for (size_t i = 0; i < workload_size; i++) {
    max_tf = workload[i]->tf > max_tf ? workload[i]->tf : max_tf;
  }

  // Print the top line
  printf("|");
  print_line(max_tf + 1);

  // Print the process names and execution timelines
  for (size_t i = 0; i < workload_size; i++) {
    printf("%-8s", workload[i]->cmd);
    for (int t = 0; t <= max_tf; t++) {
      if (workload[i]->ts <= t && t < workload[i]->ts + workload[i]->tf) {
        printf("X");
      } else {
        printf(" ");
      }
    }
    size_t actual_finish_time = workload[i]->ts + workload[i]->tf;
    printf(" (tf=%zu,idle=%zu)\n", workload[i]->tf, workload[i]->idle);
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
    schedule_processes(N);
    print_chronogram(workloads, num_workloads, CPU_CAPACITY);
    // clean_up_scheduler();
    /*
    printf("Pending Queue size: %lu\n ", get_size(pending_queue));	
    display_priority_queue(pending_queue);
    display_priority_queue(running_queue);
    */
    free(workloads);
    free_priority_queue(pending_queue);
    free_priority_queue(running_queue);
    return 0;
}