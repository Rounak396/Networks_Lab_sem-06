Assignment No 1
TCP Client Server
Write a simple TCP iterative server and client to evaluate arithmetic expressions. The
client-server system has the following functionalities:
a. The client asks for an arithmetic expression from the user. The user enters an
arithmetic expression containing only positive integers and the binary operators +, -,
*, and /. The arithmetic expression can be at most 100 characters long. There may or
may not be spaces before and after the operators. For example, all of the following
are valid expressions to enter: “13 + 42*5”, “10+2/4”, “5 + 6 - 3”. Assume that all
operators have the same precedence and are left-associative.
b. The client sends the expression to the server. You should send only the
number of bytes needed to send the expression properly and not send the complete
buffer if not needed.
c. The server computes the expression and sends the integer back to the client.
d. The client displays the result on the screen. It then prompts the user to enter
the next expression.
e. The client terminates when the user enters a –1.
You can assume that the user always enters a valid expression. To test your program,
first run the client and the server on the same machine. Use the IP address 127.0.0.1
and the port no. = 50000 + last 4 digits of your roll no. (for ex., if your roll no. is
20CS0101, use port no. = 50000 + 101 = 50101). Finally, test your program by
running the client and the server on two different machines (set the server IP address
appropriately).
You should submit two C files: the client and the iterative server. Name the files
client.c and server_it.c respectively. You must write your Full name and Roll
Number as a comment at the top of your codes.
