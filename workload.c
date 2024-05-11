#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"

#define MAX_CMD_LEN 30
#define MAX_ITEMS 20


struct workload_item_t {
    int pid;
    int ppid;
    size_t ts;
    size_t tf;
    size_t idle;
    char* cmd;
    int prio;
};


int read_workload(char *file_path) {
    FILE *fp;
    char line[100];
    workload_item *processes = NULL;
    int num_processes = 0;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), fp)) {
        workload_item process;
        char *token;
        printf("1\n");
        // Parse the line using strtok
        token = strtok(line, " \t\n"); // Split based on space, tab, or newline
        sscanf(token, "%d", &process.pid);
        printf("2\n");
        token = strtok(NULL, " \t\n");
        sscanf(token, "%d", &process.ppid);
        printf("3\n");
        token = strtok(NULL, " \t\n");
        sscanf(token, "%d", &process.prio);
        printf("4\n");
        
        token = strtok(NULL, " \t\n");
        if (token == NULL) {
            fprintf(stderr, "Error: Expected command token, but none found.\n");
        // Handle the error, such as skipping this line or returning an error code
            continue; // Skip to the next iteration of the loop
        }
        strcpy(process.cmd, token);
        printf("5\n");
        // Resize the array to accommodate the new element
        workload_item *temp = realloc(processes, sizeof(workload_item) * (num_processes + 1));
        if (temp == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            fclose(fp);
            free(processes); // Free previously allocated memory
            return EXIT_FAILURE;
        }
        processes = temp;

        // Save the process into the array and increment the count
        processes[num_processes++] = process;
    }

    fclose(fp);

    // Print out the information of all processes
    for (int i = 0; i < num_processes; i++) {
        printf("PID: %d, PPID: %d, Priority: %d, Name: %s\n",
            processes[i].pid, processes[i].ppid, processes[i].prio, processes[i].cmd);
    }

    // Free the dynamically allocated memory
    free(processes);
    return EXIT_SUCCESS;
}



int get_workload_items(char* filename, workload_item** items, size_t max_items) {
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

    item->prio = prio;

    return item;
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