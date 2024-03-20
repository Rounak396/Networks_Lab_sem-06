TCP Client Server (Concurrent)
Write a simple TCP Concurrent server and client to check whether a string of
parentheses is well balanced or not. The client-server system has the following
functionalities:
a. The client asks for a string of parentheses from the user. The user enters a
string consisting of only the characters (, ), { and }. The string can be at most 100
characters long. There may or may not be spaces between each charaters of the string.
( { ( [ ) ] } ( ) ) is not well balanced parentheses
whereas ( ( { ( ) ( [ ] ) } ) { } ) { [ ] } is well balanced.
b. The client sends the string of parentheses to the server. You should send only
the number of bytes needed to send the expression properly and not send the
complete buffer if not needed.
c. The server checks whether the string is a well balanced parentheses or not.
Then the server sends the reply (Yes or No) back to the client.
d. The client displays the result on the screen. It then prompts the user to enter
the next string.
e. The client terminates when the user enters a –1.

To test your program, first run the client and the server on the same machine. Use the
IP address 127.0.0.1 and the port no. = 50000 + last 4 digits of your roll no. (for ex.,
if your roll no. is 21CS0101, use port no. = 50000 + 101 = 50101). Finally, test your
program by running the client and the server on two different machines (set the server
IP address appropriately).
You should submit two C files: the client and the concurrent TCP server. Name the
files client.c and server_conc.c respectively. You must write your Full name and
Roll Number as a comment at the top of your codes.