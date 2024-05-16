#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"

int get_priority(const workload_item *item) {
    return item->prio;
}

void set_priority(workload_item *item, int prio) {
    item->prio = prio;
}

int get_pid(const workload_item *item) {
    return item->pid;
}

void set_pid(workload_item *item, int pid) {
    item->pid = pid;
}

size_t get_tf(const workload_item *item) {
    return item->tf;
}

void set_tf(workload_item *item, int tf) {
    item->tf = tf;
}


size_t get_ts(const workload_item *item) {
    return item->ts;
}

int get_ppid(const workload_item *item) {
    return item->ppid;
}

char* get_cmd(const workload_item *item) {
    return item->cmd;
}

size_t get_idle(const workload_item *item) {
    return item->idle;
}

void set_idle(workload_item *item, int idle) {
    item->idle = idle;
}

int get_workload_items(char* filename, workload_item* items[MAX_ITEMS], size_t max_items) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return -1;
    }

    for (int i = 0; i < max_items; i++) {
        items[i] = malloc(sizeof(workload_item));
        if (items[i] == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return -1;
        }
        items[i]->cmd = malloc(MAX_CMD_LEN * sizeof(char)); // allocate memory for cmd
    }

    int i = 0;

    while (!feof(file)) {
        if (i >= max_items) {
            printf("Warning: More than max_items in the file, some items were not retrieved.\n");
            fclose(file);
            return i;
        }

        int result = fscanf(file, "%d %d %zu %zu %zu %s %d\n", &items[i]->pid, &items[i]->ppid, &items[i]->ts, &items[i]->tf, &items[i]->idle, items[i]->cmd, &items[i]->prio);
        if (result == 7) { // all fields were successfully read
            i++;
        } else {
            free(items[i]->cmd); // if the line was not successfully read, free the allocated memory
            free(items[i]);
            printf("Warning: Could not read line %d\n", i + 1);
        }
    }

    fclose(file);
    return i; // return the number of items added to the array
}

workload_item* create_workload_item(int pid, int ppid, size_t ts, size_t tf, size_t idle, char* cmd, int prio) {
    // Allocate memory for a new workload_item
    workload_item* item = malloc(sizeof(workload_item));
    if (item == NULL) {
        printf("Memory allocation for workload_item failed\n");
        return NULL;
    }

    // Set the attributes
    item->pid = pid;
    item->ppid = ppid;
    item->ts = ts;
    item->tf = tf;
    item->idle = idle;

    // Allocate memory for cmd
    item->cmd = malloc(MAX_CMD_LEN * sizeof(char));
    if (item->cmd == NULL) {
        printf("Memory allocation for cmd failed\n");
        free(item); // Free the memory allocated for the workload_item
        return NULL;
    }

    // Copy the cmd string
    strncpy(item->cmd, cmd, MAX_CMD_LEN);
    free(item->cmd);
    
    item->prio = prio;

    return item;
}

int is_null_process(workload_item* item) {
    // Define the null process criteria
    const int null_pid = -1;
    const int null_ppid = -1;
    const size_t null_ts = 0;
    const size_t null_tf = 0;
    const size_t null_idle = 0;
    const char* null_cmd = "";
    const int null_prio = -1;

    // Check if the item matches the null process criteria
    if (item->pid == null_pid &&
        item->ppid == null_ppid &&
        item->ts == null_ts &&
        item->tf == null_tf &&
        item->idle == null_idle &&
        strcmp(item->cmd, null_cmd) == 0 &&
        item->prio == null_prio) {
        return 1;
    }
    return 0;
}

int is_equal_workload_item(workload_item* item1, workload_item* item2) {
    if (item1->pid != item2->pid) {
        return 0;
    }
    if (item1->ppid != item2->ppid) {
        return 0;
    }
    if (item1->ts != item2->ts) {
        return 0;
    }
    if (item1->tf != item2->tf) {
        return 0;
    }
    if (item1->idle != item2->idle) {
        return 0;
    }
    if (strcmp(item1->cmd, item2->cmd) != 0) {
        return 0;
    }
    if (item1->prio != item2->prio) {
        return 0;
    }
    return 1; // The items are equal
}

void free_workload_item(workload_item* item) {
    if (item != NULL) {
        if (item->cmd != NULL) {
            free(item->cmd);
        }
        free(item);
    }
}