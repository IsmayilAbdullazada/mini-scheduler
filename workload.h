#include <stdio.h>

typedef struct workload_item_t workload_item;



int get_workload_items(char* filename, workload_item* items, size_t max_items);

workload_item* create_workload_item(int pid, int ppid, size_t ts, size_t tf, size_t idle, char* cmd, int prio);


int compare_worlod_item(workload_item* item1, workload_item* item2);
