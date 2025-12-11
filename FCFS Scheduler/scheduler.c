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

void scheduler_run_main(void)
{
    int current_tick = 0;
    process_control_block_t *running_process = NULL;

    while (true) {
        apply_kill_events_at_start_of_tick(current_tick, &running_process);

        if (running_process == NULL) {
            running_process = pcb_queue_dequeue(ready_queue);
            if (running_process != NULL) {
                running_process->state = PROCESS_STATE_RUNNING;
                if (running_process->first_start_time == -1) {
                    running_process->first_start_time = current_tick;
                }
            }
        }

        bool ready_empty = pcb_queue_is_empty(ready_queue);
        bool waiting_empty = pcb_queue_is_empty(waiting_queue);
        bool running_empty = (running_process == NULL);
        bool kill_pending = kill_event_array_has_pending(kill_events);

        if (ready_empty && waiting_empty && running_empty && !kill_pending) {
            break;
        }

        bool must_move_running_to_waiting = false;

        if (running_process != NULL) {
            sleep(1);
            running_process->remaining_cpu_burst -= 1;
            if (running_process->remaining_cpu_burst < 0) running_process->remaining_cpu_burst = 0;
            running_process->executed_cpu_ticks += 1;

            if (running_process->remaining_cpu_burst == 0) {
                move_to_terminated(running_process, PROCESS_STATE_TERMINATED, current_tick + 1);
                running_process = NULL;
            } else {
                if (running_process->io_start_after_cpu >= 0 && !running_process->has_performed_io
                    && running_process->executed_cpu_ticks == running_process->io_start_after_cpu) {
                    must_move_running_to_waiting = true;
                }
            }
        } else {
            sleep(1);
        }

        update_waiting_io();

        if (must_move_running_to_waiting && running_process != NULL) {
            running_process->state = PROCESS_STATE_WAITING;
            running_process->remaining_io_duration = running_process->io_duration;
            running_process->has_performed_io = true;
            pcb_queue_enqueue(waiting_queue, running_process);
            running_process = NULL;
        }
        current_tick += 1;
    }
}


void scheduler_print_report(void)
{
    printf("\nFINAL REPORT\n");
    printf("-------------------------------------------------------------\n");
    printf("| PID  | Process Name                | CPU  | IO  | Turnaround | Waiting |\n");
    printf("-------------------------------------------------------------\n");
    while (!pcb_queue_is_empty(terminated_queue)) {
        process_control_block_t *pcb = pcb_queue_dequeue(terminated_queue);
        if (pcb == NULL) continue;
        if (pcb->state == PROCESS_STATE_KILLED) {
            printf("| %-4d | %-26s | %-4d | %-3d | %-9s | %-7s |\n",
                   pcb->process_id, pcb->process_name, pcb->total_cpu_burst, pcb->original_io_time, "KILLED", "-");
            printf("      (killed at time %d)\n", pcb->completion_time);
        } else {
            int turnaround = pcb->completion_time - pcb->arrival_time;
            int waiting_time = turnaround - pcb->total_cpu_burst;
            if (waiting_time < 0) waiting_time = 0;
            printf("| %-4d | %-26s | %-4d | %-3d | %-9d | %-7d |\n",
                   pcb->process_id, pcb->process_name, pcb->total_cpu_burst, pcb->original_io_time, turnaround, waiting_time);
        }
        free(pcb);
    }
    printf("-------------------------------------------------------------\n");
}

void scheduler_set_terminated_queue(pcb_queue_t *terminated)
{
    terminated_queue = terminated;
}
