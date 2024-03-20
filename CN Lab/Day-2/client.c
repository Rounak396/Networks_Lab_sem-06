// Name: Rounak Sarkar
// Roll: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 58118
#define MAX_EXPRESSION_LENGTH 100

void send_request(char *expression) {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Send the expression to the server
    send(client_socket, expression, strlen(expression), 0);

    // Receive and print the result from the server
    int result;
    recv(client_socket, &result, sizeof(result), 0);
    printf("Result: %s\n", result ? "Yes" : "No");

    // Close the client socket
    close(client_socket);
}

int main() {
    char expression[MAX_EXPRESSION_LENGTH];

    while (1) {
        // Get user input for the expression
        printf("Enter a string of parentheses (-1 to exit): ");
        fgets(expression, sizeof(expression), stdin);
        expression[strcspn(expression, "\n")] = 0;  // remove newline character

        // Exit the loop if the user enters -1
        if (strcmp(expression, "-1") == 0) {
            break;
        }

        // Send the expression to the server
        send_request(expression);
    }

    return 0;
}
