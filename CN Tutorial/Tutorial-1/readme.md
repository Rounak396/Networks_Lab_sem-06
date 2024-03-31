Socket in C
Sockets are low level endpoint used for processing information across a network.Common networking protocols like HTTP and FTP rely on the sockets underneath to make connection.



Client Socket Workflow
socket() | connect() | recv()
The client socket is created with a socket() call, and the connected to a remote address with the connect() call, and then finally can retrive data with recv() call.
The client socket is created with a socket() call, and the connected to a remote address with the connect() call, and then finally can retrive data with recv() call.



Server Socket Workflow
socket() | bind() | listen() | accept()
On the "server" end of the socket,we need to also create a socket with a socket() call, but we need to bind() that socket to an IP and port where it can then listen() for connections, finally accept() a connection and then send() or recv() data to the other sockets it has connected to.

