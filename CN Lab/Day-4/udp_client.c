// Name: Rounak Sarkar
// Roll No.: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define port 8080
#define MAXLINE 1024

int main(){
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("Socket Creation Failed!");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0 , sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    printf("Enter a string: ");
    fgets(buffer, MAXLINE, stdin);

    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, NULL, NULL);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);

    return 0;
}