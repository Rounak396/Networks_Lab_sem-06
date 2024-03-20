// Name: Rounak Sarkar
// Roll no.: 21CS8118

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 58114
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrSize = sizeof(serverAddr);

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        std::cerr << "Error: Unable to create socket\n";
        return -1;
    }

    memset((char *) &serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    while(1){
    char directoryPath[256];
    std::cout << "Enter the absolute path of the directory: ";
    std::cin.getline(directoryPath, sizeof(directoryPath));

    if (sendto(clientSocket, directoryPath, strlen(directoryPath), 0, (struct sockaddr *) &serverAddr, addrSize) == -1) {
        std::cerr << "Error: Unable to send data to server\n";
        close(clientSocket);
        return -1;
    }
    
    char buffer[MAX_BUFFER_SIZE];
    int bytesRead = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &serverAddr, &addrSize);
    if (bytesRead == -1) {
        std::cerr << "Error: Unable to receive data from server\n";
        close(clientSocket);
        return -1;
    }

    // Print received directory listing
    std::cout << "Files in the directory:\n";
    for (int i = 0; i < bytesRead; ++i) {
        if (buffer[i] == '\0')
            std::cout << std::endl;
        else
            std::cout << buffer[i];
    }
    }

    close(clientSocket);
    return 0;
}
