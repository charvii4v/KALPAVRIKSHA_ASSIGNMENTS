#include "data_structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void ensure_kill_capacity(kill_event_array_t *array)
{
    if (array->count < array->capacity) {
        return;
    }
    int new_capacity = (array->capacity == 0) ? 8 : (array->capacity * 2);
    kill_event_t *new_storage = (kill_event_t *) realloc(array->events, sizeof(kill_event_t) * new_capacity);
    if (new_storage == NULL) {
        perror("Failed to allocate kill_event storage");
        exit(EXIT_FAILURE);
    }
    array->events = new_storage;
    array->capacity = new_capacity;
}

process_control_block_t *pcb_create(const char *name, int pid, int cpu_burst, int io_start, int io_duration)
{
    process_control_block_t *new_pcb = (process_control_block_t *) malloc(sizeof(process_control_block_t));
    if (new_pcb == NULL) {
        perror("Failed to allocate PCB");
        exit(EXIT_FAILURE);
    }
    strncpy(new_pcb->process_name, name, PROCESS_NAME_MAX - 1);
    new_pcb->process_name[PROCESS_NAME_MAX - 1] = '\0';
    new_pcb->process_id = pid;
    new_pcb->total_cpu_burst = cpu_burst;
    new_pcb->remaining_cpu_burst = cpu_burst;
    new_pcb->io_start_after_cpu = io_start;
    new_pcb->io_duration = io_duration;
    new_pcb->remaining_io_duration = 0;
    new_pcb->executed_cpu_ticks = 0;
    new_pcb->state = PROCESS_STATE_NEW;
    new_pcb->arrival_time = 0;
    new_pcb->first_start_time = -1;
    new_pcb->completion_time = -1;
    new_pcb->original_io_time = (io_duration > 0) ? io_duration : 0;
    new_pcb->has_performed_io = false;
    new_pcb->next = NULL;
    return new_pcb;
}

void pcb_free(process_control_block_t *pcb)
{
    free(pcb);
}

pcb_queue_t *pcb_queue_create(void)
{
    pcb_queue_t *queue = (pcb_queue_t *) malloc(sizeof(pcb_queue_t));
    if (queue == NULL) {
        perror("Failed to allocate queue");
        exit(EXIT_FAILURE);
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void pcb_queue_destroy(pcb_queue_t *queue)
{
    if (queue == NULL) {
        return;
    }
    process_control_block_t *cursor = queue->head;
    while (cursor != NULL) {
        process_control_block_t *next_node = cursor->next;
        free(cursor);
        cursor = next_node;
    }
    free(queue);
}

void pcb_queue_enqueue(pcb_queue_t *queue, process_control_block_t *pcb)
{
    if (queue == NULL || pcb == NULL) {
        return;
    }
    pcb->next = NULL;
    if (queue->tail == NULL) {
        queue->head = pcb;
        queue->tail = pcb;
    } else {
        queue->tail->next = pcb;
        queue->tail = pcb;
    }
    queue->length += 1;
}

process_control_block_t *pcb_queue_dequeue(pcb_queue_t *queue)
{
    if (queue == NULL || queue->head == NULL) {
        return NULL;
    }
    process_control_block_t *node = queue->head;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    node->next = NULL;
    queue->length -= 1;
    return node;
}

bool pcb_queue_is_empty(const pcb_queue_t *queue)
{
    return (queue == NULL) || (queue->head == NULL);
}

bool pcb_queue_remove_by_pid(pcb_queue_t *queue, int process_id)
{
    if (queue == NULL || queue->head == NULL) {
        return false;
    }
    process_control_block_t *previous_node = NULL;
    process_control_block_t *cursor = queue->head;
    while (cursor != NULL) {
        if (cursor->process_id == process_id) {
            if (previous_node == NULL) {
                queue->head = cursor->next;
                if (queue->head == NULL) {
                    queue->tail = NULL;
                }
            } else {
                previous_node->next = cursor->next;
                if (previous_node->next == NULL) {
                    queue->tail = previous_node;
                }
            }
            free(cursor);
            queue->length -= 1;
            return true;
        }
        previous_node = cursor;
        cursor = cursor->next;
    }
    return false;
}

process_control_block_t *find_pcb_in_queue(const pcb_queue_t *queue, int process_id)
{
    if (queue == NULL) {
        return NULL;
    }
    process_control_block_t *cursor = queue->head;
    while (cursor != NULL) {
        if (cursor->process_id == process_id) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

kill_event_array_t *kill_event_array_create(void)
{
    kill_event_array_t *array = (kill_event_array_t *) malloc(sizeof(kill_event_array_t));
    if (array == NULL) {
        perror("Failed to allocate kill_event_array");
        exit(EXIT_FAILURE);
    }
    array->events = NULL;
    array->count = 0;
    array->capacity = 0;
    return array;
}

void kill_event_array_destroy(kill_event_array_t *array)
{
    if (array == NULL) {
        return;
    }
    free(array->events);
    free(array);
}

void kill_event_array_add(kill_event_array_t *array, int target_process_id, int kill_time)
{
    ensure_kill_capacity(array);
    array->events[array->count].target_process_id = target_process_id;
    array->events[array->count].kill_time = kill_time;
    array->count += 1;
}

kill_event_t *kill_event_array_pop_at_time(kill_event_array_t *array, int current_time, int *out_count)
{
    *out_count = 0;
    if (array == NULL || array->count == 0) {
        return NULL;
    }

    for (int outer_index = 0; outer_index < array->count - 1; ++outer_index) {
        int min_index = outer_index;
        for (int inner_index = outer_index + 1; inner_index < array->count; ++inner_index) {
            if (array->events[inner_index].kill_time < array->events[min_index].kill_time) {
                min_index = inner_index;
            }
        }
        if (min_index != outer_index) {
            kill_event_t temp = array->events[outer_index];
            array->events[outer_index] = array->events[min_index];
            array->events[min_index] = temp;
        }
    }
    int match_start = -1;
    for (int scan_index = 0; scan_index < array->count; ++scan_index) {
        if (array->events[scan_index].kill_time == current_time) {
            match_start = scan_index;
            break;
        }
        if (array->events[scan_index].kill_time > current_time) {
            break;
        }
    }
    if (match_start == -1) {
        return NULL;
    }
    int match_count = 0;
    int scan_index = match_start;
    while (scan_index < array->count && array->events[scan_index].kill_time == current_time) {
        ++match_count;
        ++scan_index;
    }
    kill_event_t *result = (kill_event_t *) malloc(sizeof(kill_event_t) * match_count);
    if (result == NULL) {
        perror("Failed to allocate pop result");
        exit(EXIT_FAILURE);
    }
    for (int copy_index = 0; copy_index < match_count; ++copy_index) {
        result[copy_index] = array->events[match_start + copy_index];
    }
    int dest_index = 0;
    for (int src_index = match_start + match_count; src_index < array->count; ++src_index) {
        array->events[dest_index++] = array->events[src_index];
    }
    array->count = dest_index;
    *out_count = match_count;
    return result;
}

bool kill_event_array_has_pending(const kill_event_array_t *array)
{
    return (array != NULL) && (array->count > 0);
}
