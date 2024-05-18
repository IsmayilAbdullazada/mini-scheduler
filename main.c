#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "workload.h"
#include "priority_queue.h"
#include "scheduler.h"

void print_chronogram(workload_item **workloads, size_t workload_size, int N) {
  int full_segments = N / 5;
  int remaining_chars = N % 5;
  printf("\t");
  for (int i = 0; i < full_segments; i++) {
      printf("|....");
  }

  if (remaining_chars) {
    printf("|");
    for (int i = 0; i < remaining_chars; i++) {
        printf(".");
    }
  }
  printf("\n");

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
  workload_item **workloads;

  workloads = malloc(MAX_ITEMS * sizeof(workload_item *));

  int num_workloads = get_workload_items("workload_input.txt", workloads, MAX_ITEMS);

  if (num_workloads == -1) {
    printf("Error: Could not read workload items from workload_input.txt.\n");
    return 1;
  }

  int N = 20;
  int cpu_capacity = 20;
  scheduler *s = init_scheduler(workloads, num_workloads, cpu_capacity);
  schedule_processes(s, N);
  print_chronogram(workloads, num_workloads, N);

  // Free resources for workloads
  for (int i = 0; i < MAX_ITEMS; i++) {
    free_workload_item(workloads[i]);
  }
  free(workloads);
  clean_up_scheduler(s);

  return 0;
}