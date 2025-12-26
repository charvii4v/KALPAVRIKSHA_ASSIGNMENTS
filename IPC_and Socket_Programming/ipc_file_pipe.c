#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define ARRAY_SIZE 5

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    int array[ARRAY_SIZE] = {5, 3, 8, 1, 9};
    printf("Original array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // File-Based IPC section
    // This part demonstrates file-based comunication between processes

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        int file_descriptor = open("temp_file", O_RDONLY);
        if (file_descriptor == -1) {
            perror("open");
            exit(1);
        }
        read(file_descriptor, array, sizeof(array));
        close(file_descriptor);
        qsort(array, ARRAY_SIZE, sizeof(int), compare);
        file_descriptor = open("temp_file", O_WRONLY | O_TRUNC);
        if (file_descriptor == -1) {
            perror("open");
            exit(1);
        }
        write(file_descriptor, array, sizeof(array));
        close(file_descriptor);
        exit(0);
    } else {
        // Parent process
        int file_descriptor = open("temp_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_descriptor == -1) {
            perror("open");
            exit(1);
        }
        write(file_descriptor, array, sizeof(array));
        close(file_descriptor);
        wait(NULL);
        file_descriptor = open("temp_file", O_RDONLY);
        if (file_descriptor == -1) {
            perror("open");
            exit(1);
        }
        read(file_descriptor, array, sizeof(array));
        close(file_descriptor);
        printf("Sorted array (File IPC): ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        unlink("temp_file");
    }

    // Pipe IPC section
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if (pid == 0) {
        // Child process
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        read(pipe_parent_to_child[0], array, sizeof(array));
        close(pipe_parent_to_child[0]);
        qsort(array, ARRAY_SIZE, sizeof(int), compare);
        write(pipe_child_to_parent[1], array, sizeof(array));
        close(pipe_child_to_parent[1]);
        exit(0);
    } else {
        // Parent process
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
        write(pipe_parent_to_child[1], array, sizeof(array));
        close(pipe_parent_to_child[1]);
        wait(NULL);
        read(pipe_child_to_parent[0], array, sizeof(array));
        close(pipe_child_to_parent[0]);
        printf("Sorted array (Pipe IPC): ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    return 0;
}