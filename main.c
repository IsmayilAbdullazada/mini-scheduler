#include <stdio.h>
#include <stdlib.h>
#include "workload.h"
#include "priority_queue.h"
#include "scheduler.h"



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
    int cpu_capacity = 20;
    scheduler *s = init_scheduler(workloads, num_workloads, cpu_capacity);
    print_chronogram(workloads, num_workloads, N);
    schedule_processes(s, N);
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

    clean_up_scheduler(s);




    return 0;
}