Client program will send a string to a Server and Server will reply the client with a message
telling whether the input string is palindrome or not. Now suppose that some clients will request
over the UDP socket, and some will connect over a TCP socket and then request. Thus, the server
now needs to open both a TCP socket and a UDP socket, and accept request from any one (using the
accept() + read()/send() call for TCP, and recvfrom() call for UDP), whichever comes first. Use the
select() call to make the server wait for any one of the two connections, and handle whichever
comes first. All handlings are iterative. [For select() system call, you may see the Beej’s Guide to
Network Programming which is already uploaded in Google classroom]
You should submit three C files, the iterative server (server.c), the UDP client (udp_client.c) and the
TCP client (tcp_clint.c).