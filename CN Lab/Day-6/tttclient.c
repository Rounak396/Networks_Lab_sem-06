// Name: Rounak Sarkar
// Roll: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT 8080

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Get server IP address
    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    // Setup server address struct
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Communication loop
    while (1)
    {
        // Read board state from server
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");

        // Tokenize received message using delimiter
        char *token = strtok(buffer, "|");
        char board_buffer[256];
        char turn_buffer[256];
        strcpy(board_buffer, token);
        token = strtok(NULL, "|");
        strcpy(turn_buffer, token);

        printf("Current Board:\n%s\n", board_buffer);

        // Handle turn message
        if (strcmp(turn_buffer, "Game over!") == 0)
        {
            printf("Game over!\n");
            break;
        }
        else if (strcmp(turn_buffer, "Your turn.") == 0)
        {
            int move;
            printf("Enter your move (1-9): ");
            scanf("%d", &move);

            // Send move to server
            bzero(buffer, 256);
            sprintf(buffer, "%d", move);
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0)
                error("ERROR writing to socket");
        }
        else
        {
            char buffer[256];
            int n = recv(sockfd, buffer, sizeof(buffer), 0);
            buffer[n] = '\0'; // Add null terminator to make it a C-string
            printf("Message from server: %s\n", buffer);
        }
    }

    // Close socket
    close(sockfd);

    return 0;
}

