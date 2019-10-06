#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

void execute_and_print(const char* alg , ScheduleResult_t sr){

    fprintf(stdout, "Scheduling Algorithm: %s\n", alg);
    fprintf(stdout, "Average Turnaround Time: %f\n", sr.average_turnaround_time);
    fprintf(stdout, "Average Waiting Time: %f\n", sr.average_waiting_time);
    fprintf(stdout, "Total Run Time: %zu\n", sr.total_run_time);
}

void execute_error(const char* alg) {
    fprintf(stderr, "%s Algorithm Execute Error.\n", alg);
}

int is_valid(const char* alg) {
    if (!strcmp(FCFS, alg)) {
        return 1;
    }
    if (!strcmp(P, alg)) {
        return 2;
    }
    if (!strcmp(RR, alg)) {
        return 3;
    }
    if (!strcmp(SJF, alg)) {
        return 4;
    }
    return -1;
}

dyn_array_t* reverse_list(dyn_array_t* current) {
    dyn_array_t* ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    size_t size = dyn_array_size(current);
    for (size_t i = 0; i < size; i++) {
        ProcessControlBlock_t pcb;
        dyn_array_extract_back(current, &pcb);
        dyn_array_push_back(ready_queue, &pcb);
    }
    return ready_queue;
}

/*
analysis <PCBs_bin_file> <schedule algorithm> [Optional_Time_Quantum]

1. Load process control blocks from binary file passed at the command line into a
dyn_array (this is your ready queue).
2. Execute your scheduling algorithm to collect the statistics
3. Clean up any allocations
4. Report your times to STDOUT (copy these to the readme.md file)
*/
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char* fname = argv[1];
    char * schedule_algo = argv[2];
    size_t quantum;

    int status = is_valid(schedule_algo);
    if (status == -1) {
        fprintf(stderr, "Error! Invalid input Algorithm %s\n", schedule_algo);
        return EXIT_FAILURE;
    }

    ScheduleResult_t sr;
    memset(&sr,0,sizeof(ScheduleResult_t));

    dyn_array_t* pcb_list = load_process_control_blocks(fname);

    if(pcb_list == NULL){
        fprintf(stderr, "Error! Can't load %s.", fname);
        exit(EXIT_FAILURE);
    }

    // revese pcb_list to extract from back
    dyn_array_t* ready_queue = reverse_list(pcb_list);
    dyn_array_destroy(pcb_list);

    if (status == 1) {
        if (first_come_first_serve(ready_queue, &sr)) {
            execute_and_print(schedule_algo, sr);
        } else {
            execute_error(schedule_algo);
        }
    }
    if (status == 2) {
        if (priority(ready_queue, &sr)) {
            execute_and_print(schedule_algo, sr);
        } else {
            execute_error(schedule_algo);
        }
    }
    if (status == 3) {
        if (argc == 4 && sscanf(argv[3], "%zu", &quantum)) {
            if (round_robin(ready_queue, &sr, quantum)) {
                execute_and_print(schedule_algo, sr);
            } else {
                execute_error(schedule_algo);
            }
        } else {
            fprintf(stderr, "Error Quantum Input\n");
        }
    }
    if (status == 4) {
        if (shortest_remaining_time_first(ready_queue, &sr)) {
            execute_and_print(schedule_algo, sr);
        } else {
            execute_error(schedule_algo);
        }
    }

    dyn_array_destroy(ready_queue);
    return EXIT_SUCCESS;
}
