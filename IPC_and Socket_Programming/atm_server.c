#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    // Function to handle client requests and manage account balence

    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) break;
        int operation;
        int amount = 0;
        sscanf(buffer, "%d %d", &operation, &amount);
        char response[BUFFER_SIZE];
        pthread_mutex_lock(&mutex);
        int file_descriptor = open("accountDB.txt", O_RDWR);
        if (file_descriptor == -1) {
            strcpy(response, "Error opening account file");
        } else {
            char balance_string[20];
            int read_bytes = read(file_descriptor, balance_string, sizeof(balance_string) - 1);
            balance_string[read_bytes] = '\0';
            int balance = atoi(balance_string);
            if (operation == 1) { // Withdraw
                if (amount > balance) {
                    strcpy(response, "Insufficient balance");
                } else {
                    balance -= amount;
                    sprintf(response, "Withdrawal successful. New balance: %d", balance);
                }
            } else if (operation == 2) { // Deposit
                balance += amount;
                sprintf(response, "Deposit successful. New balance: %d", balance);
            } else if (operation == 3) { // Display Balance
                sprintf(response, "Current balance: %d", balance);
            } else {
                strcpy(response, "Invalid operation");
            }
            if (operation == 1 || operation == 2) {
                lseek(file_descriptor, 0, SEEK_SET);
                sprintf(balance_string, "%d", balance);
                write(file_descriptor, balance_string, strlen(balance_string));
                ftruncate(file_descriptor, strlen(balance_string));
            }
            close(file_descriptor);
        }
        pthread_mutex_unlock(&mutex);
        write(client_socket, response, strlen(response));
    }
    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        exit(1);
    }
    printf("ATM Server listening on port %d\n", PORT);
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_length = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        int *client_socket_pointer = malloc(sizeof(int));
        *client_socket_pointer = client_socket;
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_socket_pointer);
        pthread_detach(thread);
    }
    close(server_socket);
    return 0;
}