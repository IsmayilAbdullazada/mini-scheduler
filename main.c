#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "workload.h"
#include "scheduler_priority_queue.h"
#include "scheduler.h"

/**
 * @brief Prints a chronogram representing the execution of workload items.
 * 
 * @param workloads An array of workload items.
 * @param workload_size The number of workload items.
 * @param N The total time units for the chronogram.
 */
void print_chronogram(workload_item **workloads, size_t workload_size, int N) {
  int full_segments = N / 5; // Calculate the number of full 5-time-unit segments
  int remaining_chars = N % 5; // Calculate the remaining characters if N is not divisible by 5
  
  printf("\t");
  
  // Print full segments of 5-time-unit intervals
  for (int i = 0; i < full_segments; i++) {
      printf("|....");
  }

  // If there are remaining characters, print them
  if (remaining_chars) {
    printf("|");
    for (int i = 0; i < remaining_chars; i++) {
        printf(".");
    }
  }
  printf("\n");
  
  for (size_t i = 0; i < workload_size; i++) {
    printf("%-8s", get_cmd(workloads[i])); // Print the command name with left alignment
    
    // Iterate over each time unit in the chronogram
    for (int t = 0; t < get_tf(workloads[i]); t++) {
      // If the current time unit is within the execution time of the workload item, print 'X', else print a space
      if (get_ts(workloads[i]) <= t && t < get_ts(workloads[i]) + get_tf(workloads[i])) {
        printf("X");
      } else {
        printf(" ");
      }
    }
    // Print finish and idle time of process
    printf("\t\t\t(tf=%lu,idle=%lu)\n", get_tf(workloads[i]), get_idle(workloads[i]));
  }
}

int main() {
  workload_item **workloads;

  workloads = malloc(MAX_ITEMS * sizeof(workload_item *));

  // Read workloads from input file
  int num_workloads = get_workload_items("workload_input.txt", workloads, MAX_ITEMS);

  if (num_workloads == -1) {
    printf("Error: Could not read workload items from workload_input.txt.\n");
    return 1;
  }

  // Timestep N = 20
  int N = 20;
  // CPU capacity C = 20
  int cpu_capacity = 20;

  // Start scheduler
  scheduler *s = init_scheduler(workloads, num_workloads, cpu_capacity);
  
  schedule_processes(s, N);

  // Print output
  print_chronogram(workloads, num_workloads, N);

  // Free resources for workloads
  for (int i = 0; i < MAX_ITEMS; i++) {
    free_workload_item(workloads[i]);
  }
  free(workloads);

  // Free scheduler
  clean_up_scheduler(s);

  return 0;
}