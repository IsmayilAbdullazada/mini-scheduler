#include <stdio.h>

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
