#include "scheduler.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct scheduler {
    priority_queue *running_queue;
    priority_queue *pending_queue;
    int cpu_capacity;
    int cpu_occupation;
};

scheduler *init_scheduler(workload_item **workloads, size_t num_events,
                          int cpu_capacity) {
    scheduler *s = malloc(sizeof(scheduler));
    if (s == NULL) {
        printf("Error: Could not allocate memory for the scheduler\n");
        exit(1);
    }

    s->running_queue = init_priority_queue(num_events);
    s->pending_queue = build_priority_queue(workloads, num_events);
    s->cpu_capacity = cpu_capacity;
    s->cpu_occupation = 0;

    if (s->running_queue == NULL || s->pending_queue == NULL) {
        printf("Error: Could not allocate memory for the priority queues\n");
        exit(1);
    }

    return s;
}

void clean_up_scheduler(scheduler *s) {
    if (s) {
        if (s->pending_queue) free_priority_queue(s->pending_queue);
        if (s->running_queue) free_priority_queue(s->running_queue);
        free(s);
    }
}

void increase_idle_tf(workload_item *process) {
    set_idle(process, get_idle(process) + 1);
    set_tf(process, get_tf(process) + 1);
}

int is_possible_process(int time, workload_item *process) {
    return time <= get_tf(process);
}

int is_current_process(int time, workload_item *process) {
    return (get_ts(process) <= time && time <= get_tf(process));
}

void schedule(scheduler *s, workload_item *process, int current_time) {
    if (process == NULL) return;
    insert(s->running_queue, process, current_time);
    delete (s->pending_queue, process, current_time);
    s->cpu_occupation += get_priority(process);
}

void deschedule(scheduler *s, workload_item *process, int current_time) {
    if (process == NULL) return;
    insert(s->pending_queue, process, current_time);
    delete (s->running_queue, process, current_time);
    s->cpu_occupation -= get_priority(process);
}

void schedule_processes(scheduler *s, int N) {
    FILE *trace_file = fopen("trace.txt", "w");
    if (trace_file == NULL) {
        printf("Error: Could not open trace file for writing\n");
        return;
    }

    for (int timestep = 0; timestep <= N; timestep++) {
        fprintf(trace_file, "[t=%d]\n", timestep);

        int index_rq = 0;
        // Process running queue first, remove finished processes
        while (index_rq < get_size(s->running_queue)) {
            workload_item *process = get_heap(s->running_queue)[index_rq++];

            if (!is_possible_process(timestep, process)) {
                fprintf(
                    trace_file,
                    "process pid=%d prio=%d ('%s') finished after time t=%d\n",
                    get_pid(process), get_priority(process), get_cmd(process),
                    timestep - 1);
                delete (s->running_queue, process, timestep);
                fprintf(trace_file, "CPU occupation: CPU[0]=%d\n",
                        s->cpu_occupation);
                index_rq = 0;
            }
        }

        // Process pending queue
        int index_pq = 0;
        while (index_pq < get_size(s->pending_queue)) {
            workload_item *pending_process = get_heap(s->pending_queue)[index_pq++];
            int in_pq = true;
            int pq_changed = false;

            if (is_current_process(timestep, pending_process) && get_priority(pending_process) <= s->cpu_capacity) {
                while ((s->cpu_occupation + get_priority(pending_process)) > s->cpu_capacity) {
                    workload_item *min_running_process = get_min(s->running_queue);

                    if (get_priority(pending_process) <= get_priority(min_running_process)) {
                        fprintf(
                            trace_file,
                            "schedule pid=%d prio=%d ('%s') ... can't fit. "
                            "Pick process to put asleep: None, as min prio: "
                            "pid=%d prio=%d ('%s') has greater priority\n",
                            get_pid(pending_process), get_priority(pending_process),
                            get_cmd(pending_process), get_pid(min_running_process),
                            get_priority(min_running_process), get_cmd(min_running_process));

                        fprintf(trace_file, "CPU occupation: CPU[0]=%d\n",s->cpu_occupation);
                        break;
                    }

                    fprintf(
                        trace_file,
                        "schedule pid=%d prio=%d ('%s') ... can't fit. "
                        "Pick process to put asleep: pid=%d prio=%d ('%s')\n",
                        get_pid(pending_process), get_priority(pending_process),
                        get_cmd(pending_process), get_pid(min_running_process),
                        get_priority(min_running_process), get_cmd(min_running_process));

                    deschedule(s, min_running_process, timestep);
                    pq_changed = true;
                    fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", s->cpu_occupation);
                }

                if (s->cpu_occupation + get_priority(pending_process) <= s->cpu_capacity) {
                    schedule(s, pending_process, timestep);
                    in_pq = false;
                    fprintf(trace_file,
                            "schedule pid=%d prio=%d ('%s') ... added to running queue\n",
                            get_pid(pending_process), get_priority(pending_process), get_cmd(pending_process));
                    fprintf(trace_file, "CPU occupation: CPU[0]=%d\n", s->cpu_occupation);
                    pq_changed = true;
                }

                if (is_current_process(timestep, pending_process) && in_pq)
                    increase_idle_tf(pending_process);
            }
            // Reset index of pending queue if pending queue changed
            if (pq_changed) index_pq = 0;
        }

        char *running_queue_str = display_priority_queue(s->running_queue, 0);
        char *pending_queue_str = display_priority_queue(s->pending_queue, 0);

        fprintf(trace_file, "running: %s\n", running_queue_str);
        fprintf(trace_file, "pending: %s\n\n", pending_queue_str);

        free(running_queue_str);
        free(pending_queue_str);
    }
    fclose(trace_file);
}
