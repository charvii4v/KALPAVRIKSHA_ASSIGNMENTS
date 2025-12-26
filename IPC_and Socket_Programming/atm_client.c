#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    // Main function to conect to server and handle user interactions

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        printf("ATM Menu:\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);
        if (choice == 4) break;

        int amount = 0;
        if (choice == 1 || choice == 2) {
            printf("Enter amount: ");
            scanf("%d", &amount);
        }

        sprintf(buffer, "%d %d", choice, amount);
        write(client_socket, buffer, strlen(buffer));

        memset(buffer, 0, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);
        printf("Server response: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
