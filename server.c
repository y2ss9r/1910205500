#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CUPS 3// Maximum cups per coffee type
#define PORT 8080   // Server port

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Array to track the number of cups sold for each coffee type
    int sold_cups[3] = {0, 0, 0}; // [0] for Espresso, [1] for Cappuccino, [2] for Latte
    char *coffee_types[3] = {"Espresso", "Cappuccino", "Latte"};

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error creating socket.\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Binding failed.\n");
        return -1;
    }

    printf("Binding successful. Listening for connections...\n");

    if (listen(server_socket, 5) < 0) {
        printf("Listening failed.\n");
        return -1;
    }

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket < 0) {
            printf("Accepting connection failed.\n");
            return -1;
        }

        printf("Client connected.\n");

        char buffer[1024];
        recv(client_socket, buffer, sizeof(buffer), 0);
        int coffee_choice = atoi(buffer); // Get client choice (0, 1, or 2)

        if (coffee_choice >= 0 && coffee_choice < 3) {
            if (sold_cups[coffee_choice] < MAX_CUPS) {
                sold_cups[coffee_choice]++;
                printf("Sold %s: %d\n", coffee_types[coffee_choice], sold_cups[coffee_choice]);

                char message[100];
                snprintf(message, sizeof(message), "Sold a cup of %s.\n", coffee_types[coffee_choice]);
                send(client_socket, message, strlen(message), 0);
            } else {
                char *message = "This coffee type is sold out for today.\n";
                send(client_socket, message, strlen(message), 0);
            }
        } else {
            char *message = "Invalid choice. Please select a coffee type between 0 and 2.\n";
            send(client_socket, message, strlen(message), 0);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
//gcc server.c -o server
//./server