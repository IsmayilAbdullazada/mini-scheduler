#include "workload.h" 
#include <assert.h>
#include <stdlib.h>

void test_get_workload_items(void) {
    workload_item** expected_workloads;
    expected_workloads = malloc(10 * sizeof(workload_item*));

    // 0 -1    0 18  0 init  10 
    // 1  0    1 16  0 bash   1 
    // 2  0    3 16  0 bash   1 
    // 3  1    4  6  0 find   2 
    // 4  1    7  9  0 gcc    5 
    // 5  4    8  9  0 ld     4  
    // 6  2   10 13  0 ssh    3 
    // 7  6   11 13  0 crypt  5 
    // 8  2   14 16  0 snake  4 
    // 9  1   14 15  0 cat    5

    expected_workloads[0] = create_workload_item(0, -1, 0, 18, 0, "init", 10);
    expected_workloads[1] = create_workload_item(1, 0, 1, 16, 0, "bash", 1);
    expected_workloads[2] = create_workload_item(2, 0, 3, 16, 0, "bash", 1);
    expected_workloads[3] = create_workload_item(3, 1, 4, 6, 0, "find", 2);
    expected_workloads[4] = create_workload_item(4, 1, 7, 9, 0, "gcc", 5);
    expected_workloads[5] = create_workload_item(5, 4, 8, 9, 0, "ld", 4);
    expected_workloads[6] = create_workload_item(6, 2, 10, 13, 0, "ssh", 3);
    expected_workloads[7] = create_workload_item(7, 6, 11, 13, 0, "crypt", 5);
    expected_workloads[8] = create_workload_item(8, 2, 14, 16, 0, "snake", 4);
    expected_workloads[9] = create_workload_item(9, 1, 14, 15, 0, "cat", 5);

    int const max_items = 20;
    workload_item** actual_workload = malloc(20 * sizeof(workload_item*));
    int num_items = get_workload_items("workload_input.txt", actual_workload, max_items);

    assert (num_items == 10);

    for (int i = 0; i < num_items; i++) {
        assert (is_equal_workload_item(expected_workloads[i], actual_workload[i]));
    }


    for (int i = 0; i < max_items; i++) {
        free_workload_item(actual_workload[i]);
    }
    free(actual_workload);

    for (int i = 0; i < 10; i++) {
        free_workload_item(expected_workloads[i]);
    }
    free(expected_workloads);
}

int main(void) {
    test_get_workload_items();
    return 0;
}