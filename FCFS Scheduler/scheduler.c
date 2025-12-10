#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pcb_queue_t *ready_queue = NULL;
static pcb_queue_t *waiting_queue = NULL;
static pcb_queue_t *terminated_queue = NULL;
static kill_event_array_t *kill_events = NULL;

void scheduler_initialize_containers(pcb_queue_t *ready, pcb_queue_t *waiting, pcb_queue_t *terminated, kill_event_array_t *events)
{
    ready_queue = ready;
    waiting_queue = waiting;
    terminated_queue = terminated;
    kill_events = events;
}

static bool remove_from_ready_or_waiting(int process_id)
{
    bool removed_ready = pcb_queue_remove_by_pid(ready_queue, process_id);
    bool removed_waiting = pcb_queue_remove_by_pid(waiting_queue, process_id);
    return removed_ready || removed_waiting;
}

static void move_to_terminated(process_control_block_t *pcb, process_state_t final_state, int completion_time)
{
    if (pcb == NULL) return;
    pcb->state = final_state;
    pcb->completion_time = completion_time;
    pcb->next = NULL;
    pcb_queue_enqueue(terminated_queue, pcb);
}
