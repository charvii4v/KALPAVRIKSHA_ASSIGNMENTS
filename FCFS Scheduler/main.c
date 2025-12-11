#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void scheduler_initialize_containers(pcb_queue_t *, pcb_queue_t *, pcb_queue_t *, kill_event_array_t *);
void scheduler_run_main(void);
void scheduler_print_report(void);
void scheduler_set_terminated_queue(pcb_queue_t *);

// trims whitespace from begining and end of string
static void trim_whitespace(char *text)
{
    if (text == NULL) return;
    char *start = text;
    while (isspace((unsigned char) *start)) start++;
    if (start != text) memmove(text, start, strlen(start) + 1);
    size_t length = strlen(text);
    while (length > 0 && isspace((unsigned char) text[length - 1])) { text[length - 1] = '\0'; length--; }
}


// parses a line and extracts proccess paremeters from input
static int parse_process(const char *line, char *out_name, int *out_pid, int *out_cpu, int *out_io_start, int *out_io_duration)
{
    char name_token[PROCESS_NAME_MAX];
    char pid_token[32];
    char cpu_token[32];
    char io_start_token[32];
    char io_duration_token[32];
    int scanned = sscanf(line, "%63s %31s %31s %31s %31s",
                         name_token, pid_token, cpu_token, io_start_token, io_duration_token);
    if (scanned < 3) return 0;
    if (strcasecmp(name_token, "KILL") == 0) return 0;
    strncpy(out_name, name_token, PROCESS_NAME_MAX - 1);
    out_name[PROCESS_NAME_MAX - 1] = '\0';
    *out_pid = atoi(pid_token);
    *out_cpu = atoi(cpu_token);
    if (scanned >= 5) {
        if (strcmp(io_start_token, "-") == 0) *out_io_start = -1; else *out_io_start = atoi(io_start_token);
        if (strcmp(io_duration_token, "-") == 0) *out_io_duration = 0; else *out_io_duration = atoi(io_duration_token);
    } else {
        *out_io_start = -1;
        *out_io_duration = 0;
    }
    return 1;
}


int main(void)
{
    pcb_queue_t *ready = pcb_queue_create();
    pcb_queue_t *waiting = pcb_queue_create();
    pcb_queue_t *terminated = pcb_queue_create();
    kill_event_array_t *kill_events = kill_event_array_create();

    char input_line[256];
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        trim_whitespace(input_line);
        if (strlen(input_line) == 0) continue;
        char first_token[8];
        int first_scanned = sscanf(input_line, "%7s", first_token);
        if (first_scanned <= 0) continue;
        if (strcasecmp(first_token, "KILL") == 0) {
            char kill_token[8];
            int kill_pid = 0;
            int kill_time = 0;
            int parsed = sscanf(input_line, "%7s %d %d", kill_token, &kill_pid, &kill_time);
            if (parsed == 3) kill_event_array_add(kill_events, kill_pid, kill_time);
            else fprintf(stderr, "Warning: malformed KILL line: %s\n", input_line);
            continue;
        }
        char pname[PROCESS_NAME_MAX];
        int pid = 0;
        int cpu_burst = 0;
        int io_start = -1;
        int io_duration = 0;
        int ok = parse_process(input_line, pname, &pid, &cpu_burst, &io_start, &io_duration);
        if (!ok) { fprintf(stderr, "Warning: malformed process line: %s\n", input_line); continue; }
        if (io_start < 0 || io_duration <= 0) { io_start = -1; io_duration = 0; }
        process_control_block_t *pcb = pcb_create(pname, pid, cpu_burst, io_start, io_duration);
        pcb->state = PROCESS_STATE_READY;
        pcb_queue_enqueue(ready, pcb);
    }

    scheduler_initialize_containers(ready, waiting, terminated, kill_events);
    scheduler_set_terminated_queue(terminated);

    scheduler_run_main();

    scheduler_print_report();

    pcb_queue_destroy(ready);
    pcb_queue_destroy(waiting);
    kill_event_array_destroy(kill_events);
    
    return 0;
}
