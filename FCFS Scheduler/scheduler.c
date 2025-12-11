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

static void apply_kill_events_at_start_of_tick(int current_tick, process_control_block_t **running_pointer)
{
    int popped_count = 0;
    kill_event_t *events = kill_event_array_pop_at_time(kill_events, current_tick, &popped_count);
    if (events == NULL || popped_count == 0) return;
    for (int index = 0; index < popped_count; ++index) {
        int target_pid = events[index].target_process_id;
        if (*running_pointer != NULL && (*running_pointer)->process_id == target_pid) {
            move_to_terminated(*running_pointer, PROCESS_STATE_KILLED, current_tick);
            *running_pointer = NULL;
            continue;
        }
        remove_from_ready_or_waiting(target_pid);
    }
    free(events);
}

static int update_waiting_io(void)
{
    if (pcb_queue_is_empty(waiting_queue)) return;
    pcb_queue_t *temporary = pcb_queue_create();
    while (!pcb_queue_is_empty(waiting_queue)) {
        process_control_block_t *pcb = pcb_queue_dequeue(waiting_queue);
        if (pcb == NULL) continue;
        if (pcb->remaining_io_duration > 0) {
            pcb->remaining_io_duration -= 1;
        }
        if (pcb->remaining_io_duration == 0) {
            pcb->state = PROCESS_STATE_READY;
            pcb_queue_enqueue(ready_queue, pcb);
        } else {
            pcb_queue_enqueue(temporary, pcb);
        }
    }

    while (!pcb_queue_is_empty(temporary)) {
        process_control_block_t *pcb = pcb_queue_dequeue(temporary);
        pcb_queue_enqueue(waiting_queue, pcb);
    }
    pcb_queue_destroy(temporary);
}
