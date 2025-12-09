#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "pcb.h"
#include <stdbool.h>

typedef struct queue_node {
    process_control_block_t *process_block;
    struct queue_node *next;
} queue_node_t;

typedef struct simple_queue {
    queue_node_t *head;
    queue_node_t *tail;
    int length;
} simple_queue_t;

simple_queue_t *queue_create(void);
void queue_destroy(simple_queue_t *queue);
bool queue_is_empty(const simple_queue_t *queue);
void queue_enqueue(simple_queue_t *queue, process_control_block_t *process_block);
process_control_block_t *queue_dequeue(simple_queue_t *queue);
bool queue_remove_by_pid(simple_queue_t *queue, int process_id);

typedef struct pid_hashmap pid_hashmap_t;

pid_hashmap_t *pid_hashmap_create(int suggested_bucket_count);
void pid_hashmap_destroy(pid_hashmap_t *map);
bool pid_hashmap_put(pid_hashmap_t *map, process_control_block_t *process_block);
process_control_block_t *pid_hashmap_get(pid_hashmap_t *map, int process_id);
bool pid_hashmap_remove(pid_hashmap_t *map, int process_id);

typedef struct kill_event {
    int target_process_id;
    int kill_time;
} kill_event_t;

typedef struct kill_event_list kill_event_list_t;

kill_event_list_t *kill_event_list_create(void);
void kill_event_list_destroy(kill_event_list_t *list);
void kill_event_list_add(kill_event_list_t *list, int target_process_id, int kill_time);
kill_event_t *kill_event_list_pop_events_at_time(kill_event_list_t *list, int current_time, int *out_count);
bool kill_event_list_has_pending_events(const kill_event_list_t *list);

#endif
