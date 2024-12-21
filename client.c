#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080 // Server port

int main() {
    printf("Client program started...\n");
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error creating client socket.\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed.\n");
        return -1;
    } else {
        printf("Connected to the server successfully.\n");
    }

    // Request coffee choice from client
    printf("Choose a coffee type:\n");
    printf("0 - Espresso\n");
    printf("1 - Cappuccino\n");
    printf("2 - Latte\n");

    int coffee_choice;
    scanf("%d", &coffee_choice);

    // Convert the client's choice to a string and send it to the server
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%d", coffee_choice);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive response from the server
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Message from server: %s\n", buffer);

    close(client_socket);
    return 0;
}
//gcc client.c -o client
//./client
