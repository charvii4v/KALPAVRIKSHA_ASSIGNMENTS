#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdbool.h>

#define PROCESS_NAME_MAX 64

typedef enum {
    PROCESS_STATE_NEW,
    PROCESS_STATE_READY,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_WAITING,
    PROCESS_STATE_TERMINATED,
    PROCESS_STATE_KILLED
} process_state_t;

typedef struct process_control_block {
    char process_name[PROCESS_NAME_MAX];
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
    struct process_control_block *next;
} process_control_block_t;

typedef struct pcb_queue {
    process_control_block_t *head;
    process_control_block_t *tail;
    int length;
} pcb_queue_t;

typedef struct kill_event {
    int target_process_id;
    int kill_time;
} kill_event_t;

typedef struct kill_event_array {
    kill_event_t *events;
    int count;
    int capacity;
} kill_event_array_t;

process_control_block_t *pcb_create(const char *name, int pid, int cpu_burst, int io_start, int io_duration);
void pcb_free(process_control_block_t *pcb);

pcb_queue_t *pcb_queue_create(void);
void pcb_queue_destroy(pcb_queue_t *queue);
void pcb_queue_enqueue(pcb_queue_t *queue, process_control_block_t *pcb);
process_control_block_t *pcb_queue_dequeue(pcb_queue_t *queue);
bool pcb_queue_remove_by_pid(pcb_queue_t *queue, int process_id);
bool pcb_queue_is_empty(const pcb_queue_t *queue);

kill_event_array_t *kill_event_array_create(void);
void kill_event_array_destroy(kill_event_array_t *array);
void kill_event_array_add(kill_event_array_t *array, int target_process_id, int kill_time);
kill_event_t *kill_event_array_pop_at_time(kill_event_array_t *array, int current_time, int *out_count);
bool kill_event_array_has_pending(const kill_event_array_t *array);

process_control_block_t *find_pcb_in_queue(const pcb_queue_t *queue, int process_id);

#endif
