#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"

struct workload_item_t {
      int pid;       //< the event id
      int ppid;      //< the event parent id
      size_t ts;     //< start date
      size_t tf;     //< finish date
      size_t idle;   //< total time the process has been idle;
      char *cmd;     //< the binary name
      int prio;      //< process priority
};

workload_item workload[] = {
//  pid ppid  ts  tf idle  cmd     prio
    {0, -1,    0, 18,  0, "init",  10 },
    {1,  0,    1, 16,  0, "bash",   1 },
    {2,  0,    3, 16,  0, "bash",   1 },
    {3,  1,    4,  6,  0, "find",   2 },
    {4,  1,    7,  9,  0, "gcc",    5 },
    {5,  4,    8,  9,  0, "ld",     4 }, 
    {6,  2,   10, 13,  0, "ssh",    3 },
    {7,  6,   11, 13,  0, "crypt",  5 },
    {8,  2,   14, 16,  0, "snake",  4 },
    {9,  1,   14, 15,  0, "cat",    5 },
};

size_t workload_size = sizeof(workload)/sizeof(workload[0]);

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

int main(void) {
    read_workload("workload_input.txt");
    return 0;
}