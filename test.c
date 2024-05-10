#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    FILE *fp;
    char line[MAX_LINE_LENGTH];
    struct Process processes[MAX_PROCESSES];
    int num_processes = 0;

    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), fp) && num_processes < MAX_PROCESSES) {
        struct Process process;
        char *token;

        // Parse the line using strtok
        token = strtok(line, " \t\n"); // Split based on space, tab, or newline
        sscanf(token, "%d", &process.pid);

        token = strtok(NULL, " \t\n");
        sscanf(token, "%d", &process.ppid);

        token = strtok(NULL, " \t\n");
        sscanf(token, "%d", &process.priority);

        token = strtok(NULL, " \t\n");
        strcpy(process.name, token);

        // Save the process into the array
        processes[num_processes++] = process;
    }

    fclose(fp);

    // Print out the information of all processes
    for (int i = 0; i < num_processes; i++) {
        printf("PID: %d, PPID: %d, Priority: %d, Name: %s\n",
               processes[i].pid, processes[i].ppid, processes[i].priority, processes[i].name);
    }

    return EXIT_SUCCESS;

void read_workload(char *file_path) {
    FILE *fp;
    char *file_content = NULL;
    char *workload_item = NULL, *workload_entity = NULL;
    char c;
    int i = 0;
    if((fp = fopen(file_path, "r")) == NULL) {
        printf("Error Opening File\n");
        exit(1);
    } 
    while ((c = fgetc(fp))!= EOF) {
        if (c=='\n')
          c = ' ';
        workload_item = (char *)realloc(workload_item, ++i*sizeof(char));
        workload_item[i-1]=c;
        if (i==2) {
          for (int j=0; j<i; j++) {
            printf("%s\n", workload_item);
          }
        }
        workload_entity = strtok(workload_item, " ");
        if (i==5) printf("%s", workload_entity);
    }

    fclose(fp);
}

int main(void) {
  char* file_path = "workload_input.txt";

  read_workload(file_path);
}