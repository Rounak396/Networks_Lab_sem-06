// Name: Rounak Sarkar
// Roll No.: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define PORT 8080
#define MAXLINE 1024

int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int tcp_fd, udp_fd, max_fd, activity, client_len, n;
    char buffer[MAXLINE];
    struct sockaddr_in server_addr, client_addr;

    // Create TCP socket
    if ((tcp_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind TCP socket
    if (bind(tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    if (bind(udp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for TCP connections
    if (listen(tcp_fd, 5) == -1) {
        perror("TCP listen failed");
        exit(EXIT_FAILURE);
    }

    fd_set readfds;

    printf("Server is running...\n");

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(tcp_fd, &readfds);
        FD_SET(udp_fd, &readfds);
        max_fd = (tcp_fd > udp_fd) ? tcp_fd : udp_fd;

        // Wait for activity on either TCP or UDP socket
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Handle TCP connection
        if (FD_ISSET(tcp_fd, &readfds)) {
            int new_tcp_fd = accept(tcp_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
            if (new_tcp_fd < 0) {
                perror("TCP accept failed");
                exit(EXIT_FAILURE);
            }
            printf("TCP connection accepted\n");

            n = read(new_tcp_fd, buffer, MAXLINE);
            buffer[n] = '\0';

            if (isPalindrome(buffer)) {
                send(new_tcp_fd, "Palindrome\n", 12, 0);
            } else {
                send(new_tcp_fd, "Not palindrome\n", 16, 0);
            }

            close(new_tcp_fd);
        }

        // Handle UDP connection
        if (FD_ISSET(udp_fd, &readfds)) {
            client_len = sizeof(client_addr);
            n = recvfrom(udp_fd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
            buffer[n] = '\0';

            if (isPalindrome(buffer)) {
                sendto(udp_fd, "Palindrome\n", 12, 0, (struct sockaddr *)&client_addr, client_len);
            } else {
                sendto(udp_fd, "Not palindrome\n", 16, 0, (struct sockaddr *)&client_addr, client_len);
            }
        }
    }

    close(tcp_fd);
    close(udp_fd);

    return 0;
}
