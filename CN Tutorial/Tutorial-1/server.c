#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    char server_message[256] = "You have reached the server!";

    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Socket function is called and the result is stored in a variable

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    // bind() function is used to bind the socket to the address and port number specified in server_address.
    // bind() takes 3 parameters:
    // 1. The socket file descriptor, 2. The address of the server, 3. The size of the server address.

    int status = listen(server_socket, 5);
    // listen() function is used to listen for incoming connections.
    // listen() returns 0 if the operation is successful, i.e. if the server is listening, -1 otherwise.
    // listen() takes 2 parameters:
    // 1. The socket file descriptor, 2. The maximum number of connections that can be queued (queueLen).
    // queueLen is the maximum number of connections that can be waiting while the process is handling a particular connection.
    if (status == 0){
        printf("Listening for incoming connections...\n");
    } else {
        printf("Error occurred while listening for incoming connections\n");
    }

    int client_socket = accept(server_socket, NULL, NULL);
    // accept() function is used to accept the incoming connection.
    // accept() returns a new socket file descriptor for the accepted connection.
    // accept() takes 3 parameters:
    // 1. The socket file descriptor, 2. The address of the client, 3. The size of the client address.

    send(client_socket, server_message, sizeof(server_message), 0);
    // send() function is used to send data to the client.
    // send() returns the number of bytes sent.
    // send() takes 4 parameters:
    // 1. The socket file descriptor, 2. The data to be sent, 3. The size of the data, 4. Flags

    // Close the socket
    close(server_socket);

    return 0;
}