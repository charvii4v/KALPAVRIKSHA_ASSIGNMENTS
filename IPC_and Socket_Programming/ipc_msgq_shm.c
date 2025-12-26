#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define ARRAY_SIZE 5

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

struct message_buffer {
    long message_type;
    int array[ARRAY_SIZE];
};

int main() {
    int array[ARRAY_SIZE] = {5, 3, 8, 1, 9};
    printf("Original array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Message Queue IPC section
    // Demonstrates interprocess comunication using message queues

    key_t key = 1234;
    int message_queue_id = msgget(key, 0666 | IPC_CREAT);
    if (message_queue_id == -1) {
        perror("msgget");
        exit(1);
    }
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        struct message_buffer message;
        if (msgrcv(message_queue_id, &message, sizeof(message.array), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        memcpy(array, message.array, sizeof(array));
        qsort(array, ARRAY_SIZE, sizeof(int), compare);
        message.message_type = 2;
        memcpy(message.array, array, sizeof(array));
        if (msgsnd(message_queue_id, &message, sizeof(message.array), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
        exit(0);
    } else {
        // Parent process
        struct message_buffer message;
        message.message_type = 1;
        memcpy(message.array, array, sizeof(array));
        if (msgsnd(message_queue_id, &message, sizeof(message.array), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
        wait(NULL);
        if (msgrcv(message_queue_id, &message, sizeof(message.array), 2, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        memcpy(array, message.array, sizeof(array));
        printf("Sorted array (Message Queue IPC): ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        if (msgctl(message_queue_id, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(1);
        }
    }

    // Shared Memory IPC section
    int shared_memory_id = shmget(key, sizeof(array), 0666 | IPC_CREAT);
    if (shared_memory_id == -1) {
        perror("shmget");
        exit(1);
    }
    pid = fork();
    if (pid == 0) {
        // Child process
        int *shared_array = (int *)shmat(shared_memory_id, NULL, 0);
        if (shared_array == (int *)-1) {
            perror("shmat");
            exit(1);
        }
        qsort(shared_array, ARRAY_SIZE, sizeof(int), compare);
        if (shmdt(shared_array) == -1) {
            perror("shmdt");
            exit(1);
        }
        exit(0);
    } else {
        // Parent process
        int *shared_array = (int *)shmat(shared_memory_id, NULL, 0);
        if (shared_array == (int *)-1) {
            perror("shmat");
            exit(1);
        }
        memcpy(shared_array, array, sizeof(array));
        wait(NULL);
        memcpy(array, shared_array, sizeof(array));
        printf("Sorted array (Shared Memory IPC): ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        if (shmdt(shared_array) == -1) {
            perror("shmdt");
            exit(1);
        }
        if (shmctl(shared_memory_id, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }

    return 0;
}