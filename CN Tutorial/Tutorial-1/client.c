#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int network_socket = socket(AF_INET, SOCK_STREAM,0);
    // Socket function is called and the result is stored 
    // in a variable.
    // parameters for socket syscall
    // domainOfTheSocket, typeOfTheSocket, FlagForProtocol
    // AF_INET = constant defined in the header file for us
    // SOCK_STREAM-->For TCP,  SOCK_DGRAM-->For UDP
    // flag=0 for TCP (default protocol)


    struct sockaddr_in server_address;
    // sockaddr_in is a structure defined in the header file
    // which is used to store the address of the server
    // It is used for creating network connection
    // In order to connect to the other side of the socket we need to declare connect with specifying the address where we want to
    // connect to already defined struct sockaddr_in.

    server_address.sin_family = AF_INET; // Address family: AF_INET
    // This is the address family for IPv4 Internet protocols.

    server_address.sin_port = htons(9002);
    // This is the port number of the server that we want to connect to.
    // htons() function is used to convert the port number to the network byte order.
    // This is because the port number is always in the host byte order. Hence we need to convert it to network byte order.

    server_address.sin_addr.s_addr = INADDR_ANY;
    // This is the IP address of the server that we want to connect to.
    // INADDR_ANY is used when we don't know the IP address of the server.
    // It is used to connect to the localhost.

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // connect() function is used to connect to the server.
    // connect() returns an intger signifying the status of the connection.
    // connect() takes 3 parameters:
    // 1. The socket file descriptor, 2. The address of the server, 3. The size of the server address.

    if(connection_status == -1){
        printf("There was an error making a connection to the remote socket\n\n");
    }

    char server_response[256]; // A character array to store the server response

    recv(network_socket, &server_response, sizeof(server_response), 0);
    // recv() function is used to receive data from the server.
    // recv() returns the number of bytes received.
    // recv() takes 4 parameters:
    // 1. The socket file descriptor, 2. The buffer where the data will be stored, 3. The size of the buffer, 4. Flags
    // Flag is 0 which means that the data is received with the default behaviour.
    // If the flag is 0, the recv() function will wait for the data to arrive.
    // If the flag is set to MSG_DONTWAIT, the recv() function will return immediately even if there is no data to be received.

    printf("The server sent the data: %s\n", server_response);

    close(network_socket);
    // close() function is used to close the socket.
    // close() takes the socket file descriptor as a parameter.

    return 0;
}