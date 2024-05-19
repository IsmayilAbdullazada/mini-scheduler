#include <stdio.h>

#define MAX_CMD_LEN 30
#define MAX_ITEMS 20

/**
* @brief Structure to represent a workload item.
*/
typedef struct workload_item_t workload_item;

/**
* @brief Retrieve workload items from a file.
* @param filename Name of the file containing workload items.
* @param items Array to store the workload items.
* @param max_items Maximum number of items to retrieve.
* @return The number of items retrieved, or -1 on error.
*/
int get_workload_items(char* filename, workload_item** items, size_t max_items);

/**
* @brief Create a workload item.
* @param pid Process ID.
* @param ppid Parent process ID.
* @param ts Start time.
* @param tf Finish time.
* @param idle Idle time.
* @param cmd Command.
* @param prio Priority.
* @return Pointer to the created workload item, or NULL on error.
*/
workload_item* create_workload_item(int pid, int ppid, size_t ts, size_t tf, size_t idle, char* cmd, int prio);

/**
* @brief Compare two workload items for equality.
* @param item1 First workload item.
* @param item2 Second workload item.
* @return 1 if items are equal, 0 otherwise.
*/
int is_equal_workload_item(workload_item* item1, workload_item* item2);

/**
* @brief Free a workload item.
* @param item Pointer to the workload item to free.
*/
void free_workload_item(workload_item *item);

/**
* @brief Get the priority of a workload item.
* @param item Pointer to the workload item.
* @return The priority of the workload item.
*/
int get_priority(const workload_item *item);

/**
* @brief Set the priority of a workload item.
* @param item Pointer to the workload item.
* @param prio The new priority.
*/
void set_priority(workload_item *item, int prio);

/**
* @brief Get the process ID of a workload item.
* @param item Pointer to the workload item.
* @return The process ID of the workload item.
*/
int get_pid(const workload_item *item);

/**
* @brief Set the process ID of a workload item.
* @param item Pointer to the workload item.
* @param pid The new process ID.
*/
void set_pid(workload_item *item, int pid);

/**
* @brief Get the finish time of a workload item.
* @param item Pointer to the workload item.
* @return The finish time of the workload item.
*/
size_t get_tf(const workload_item *item);

/**
* @brief Set the finish time of a workload item.
* @param item Pointer to the workload item.
* @param tf The new finish time.
*/
void set_tf(workload_item *item, int tf);

/**
* @brief Get the start time of a workload item.
* @param item Pointer to the workload item.
* @return The start time of the workload item.
*/
size_t get_ts(const workload_item *item);

/**
* @brief Get the parent process ID of a workload item.
* @param item Pointer to the workload item.
* @return The parent process ID of the workload item.
*/
int get_ppid(const workload_item *item);

/**
* @brief Get the command of a workload item.
* @param item Pointer to the workload item.
* @return The command of the workload item.
*/
char* get_cmd(const workload_item *item);

/**
* @brief Get the idle time of a workload item.
* @param item Pointer to the workload item.
* @return The idle time of the workload item.
*/
size_t get_idle(const workload_item *item);

/**
* @brief Set the idle time of a workload item.
* @param item Pointer to the workload item.
* @param idle The new idle time.
*/
void set_idle(workload_item *item, int idle);