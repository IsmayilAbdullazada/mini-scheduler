#include <stdio.h>

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

typedef struct workload_item_t workload_item;

int get_workload_items(char* filename, workload_item** items, size_t max_items);

workload_item* create_workload_item(int pid, int ppid, size_t ts, size_t tf, size_t idle, char* cmd, int prio);

int is_equal_workload_item(workload_item* item1, workload_item* item2);

void free_workload_item(workload_item* item);

int is_null_process(workload_item *item);

int is_higher_priority(workload_item a, workload_item b);

int get_priority(const workload_item *item);
void set_priority(workload_item *item, int prio);
int get_pid(const workload_item *item);
void set_pid(workload_item *item, int pid);
size_t get_tf(const workload_item *item);
void set_tf(workload_item *item, int tf);
size_t get_ts(const workload_item *item);
int get_ppid(const workload_item *item);
char* get_cmd(const workload_item *item);
size_t get_idle(const workload_item *item);
void set_idle(workload_item *item, int idle);