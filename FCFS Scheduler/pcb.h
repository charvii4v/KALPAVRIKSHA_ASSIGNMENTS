#ifndef PCB_H
#define PCB_H

#include <stdbool.h>

#define PROCESS_NAME_MAX_LENGTH 64

typedef enum {
    PROCESS_STATE_NEW,
    PROCESS_STATE_READY,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_WAITING,
    PROCESS_STATE_TERMINATED,
    PROCESS_STATE_KILLED
} process_state_t;

typedef struct process_control_block {
    char process_name[PROCESS_NAME_MAX_LENGTH];
    int process_id;
    int total_cpu_burst;
    int remaining_cpu_burst;
    int io_start_after_cpu;
    int io_duration;
    int remaining_io_duration;
    int executed_cpu_ticks;
    process_state_t state;
    int arrival_time;
    int first_start_time;
    int completion_time;
    int original_io_time;          
    bool has_performed_io;
    struct process_control_block *next_in_list;
} process_control_block_t;

process_control_block_t *pcb_create(const char *process_name_arg,
                                    int process_id_arg,
                                    int total_cpu_burst_arg,
                                    int io_start_after_cpu_arg,
                                    int io_duration_arg);

void pcb_print_debug(const process_control_block_t *process_block);

#endif 
