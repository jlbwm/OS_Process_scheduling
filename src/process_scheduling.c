#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


#include "dyn_array.h"
#include "processing_scheduling.h"


// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

/*
    typedef struct
    {
        uint32_t remaining_burst_time; // the remaining burst of the pcb
        uint32_t priority;             // The priority of the task
        uint32_t arrival;              // Time the process arrived in the ready queue
        bool started;                  // If it has been activated on virtual CPU
    } ProcessControlBlock_t;           // you may or may not need to add more elements

    typedef struct
    {
        float average_waiting_time;    // the average waiting time in the ready queue until first schedue on the cpu
        float average_turnaround_time; // the average completion time of the PCBs
        unsigned long total_run_time;  // the total time to process all the PCBs in the ready queue
    } ScheduleResult_t;
    */
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    if (ready_queue == NULL || result == NULL) {
        return false;
    }
    size_t total_pcb = dyn_array_size(ready_queue);
    uint32_t total_run_time = 0;
    uint32_t total_completion_time = 0;
    uint32_t total_waiting_time = 0;

    for (size_t i = 0; i < total_pcb; ++i) {
        ProcessControlBlock_t curt_pcb; 
        dyn_array_extract_back(ready_queue, &curt_pcb);
        total_waiting_time += total_run_time - curt_pcb.arrival;
        total_run_time += curt_pcb.remaining_burst_time;
        total_completion_time += total_run_time;
    }

    result->total_run_time = total_run_time;
    result->average_waiting_time = (float)total_waiting_time / total_pcb;
    result->average_turnaround_time = (float)total_completion_time / total_pcb;
    
    return true;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
   return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
    return false;
}

/*
PCB Structure:
number of process control blocks (32-bit unsigned)
first process burst time
first process priority
first process arrival
second process burst time
second process priority
second process arrival
*/
dyn_array_t *load_process_control_blocks(const char *input_file) {
    // null file path
    if(input_file == NULL){
        return NULL;
    }
    // not found file
    int fd = open(input_file, O_RDONLY);
    if (fd <= 0) {
        return NULL;
    }
    // empty found file
    uint32_t num_of_pcb;
    ssize_t size = read(fd, &num_of_pcb, sizeof(uint32_t));
    // printf("size: %d\n", size); size = 4 (32bit)
    if (size <= 0) {
        close(fd);
        return NULL;
    }

    // incorrectPCBFoundFile
    struct stat buf;
    fstat(fd, &buf);
    off_t count = buf.st_size;
    uint32_t actual_num_of_pcb = (count - size) / (sizeof(uint32_t) * 3);
    if (num_of_pcb != actual_num_of_pcb) {
        return NULL;
    }

    uint32_t* buffer = (uint32_t*)malloc(num_of_pcb * 3 * sizeof(uint32_t));
    for (ssize_t i = 0; i < num_of_pcb * 3; i++) {
        read(fd, &buffer[i], sizeof(uint32_t));
    }
    close(fd);

    ProcessControlBlock_t* pcbs = (ProcessControlBlock_t*)malloc(num_of_pcb * sizeof(ProcessControlBlock_t));
    int index = 0;
    for (uint32_t i = 0; i < num_of_pcb * 3; i+=3) {
        pcbs[index].remaining_burst_time = buffer[i];
        pcbs[index].priority = buffer[i+1];
        pcbs[index].arrival = buffer[i+2];
        index++;
    }
    free(buffer);

    dyn_array_t* ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    if (!ready_queue) {
        free(pcbs);
        return NULL;
    }

    for(size_t i = 0; i < num_of_pcb; i++){
        // dyn_array_push_back(ready_queue, pcbs + i);
        // ProcessControlBlock_t *pcb = pcbs+i;
        // printf("index: %d, %d, %d, %d\n", i, pcb->remaining_burst_time, pcb->priority, pcb->arrival);  
        if(!dyn_array_push_back(ready_queue, (void*)(pcbs + i))){
            free(pcbs);
            dyn_array_destroy(ready_queue);
            return NULL;
        }
    }
    free(pcbs);
    return ready_queue;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    return false;
}
