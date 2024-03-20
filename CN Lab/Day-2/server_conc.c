// Name: Rounak Sarkar
// Roll: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 58118
#define MAX_EXPRESSION_LENGTH 100


int is_matching(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '{' && closing == '}') ||
           (opening == '[' && closing == ']');
}

int is_well_balanced(char *parentheses) {
    char stack[MAX_EXPRESSION_LENGTH];
    int top = -1;
    // we simply iterate on every charcter of input
    for (int i = 0; i < strlen(parentheses); i++) {
        if (parentheses[i] == '(' || parentheses[i] == '{' || parentheses[i] == '[') {
            stack[++top] = parentheses[i];
        } else if (parentheses[i] == ')' || parentheses[i] == '}' || parentheses[i] == ']') {
            if (top == -1 || !is_matching(stack[top--], parentheses[i])) {
                return 0;
            }
        }
    }

    return top == -1;
}



void handle_client(int client_socket) {
    char buffer[MAX_EXPRESSION_LENGTH];
    recv(client_socket, buffer, sizeof(buffer), 0); // intializing the buffer with the input we got client side in socket

    if (strcmp(buffer, "-1") == 0) {
        printf("Client terminated.\n");
        close(client_socket);
        return;
    }

    int result = is_well_balanced(buffer);
    send(client_socket, &result, sizeof(result), 0);
    close(client_socket);
}

int main() {
    //creating server tools
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Server listening on port %d...\n", PORT);

    char buffer[MAX_EXPRESSION_LENGTH];  // Declare buffer here

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle client request
        handle_client(client_socket);

        // Check if the client requested termination
        if (strcmp(buffer, "-1") == 0) {
            printf("Server terminated by client request.\n");
            break;  // Exit the server loop
        }
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
