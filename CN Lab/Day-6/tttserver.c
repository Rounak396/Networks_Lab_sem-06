// Name: Rounak Sarkar
// Roll: 21CS8118

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>

#define SHMSZ 10  // Size of shared memory segment
#define PORT 8080 // Port number

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int check_winner(char *board)
{
    // Check rows
    for (int i = 1; i <= 7; i += 3)
    {
        if (board[i] != ' ' && board[i] == board[i + 1] && board[i] == board[i + 2])
            return (board[i] == 'X') ? 1 : 2; // Player 1 or Player 2 wins
    }

    // Check columns
    for (int i = 1; i <= 3; i++)
    {
        if (board[i] != ' ' && board[i] == board[i + 3] && board[i] == board[i + 6])
            return (board[i] == 'X') ? 1 : 2; // Player 1 or Player 2 wins
    }

    // Check diagonals
    if (board[1] != ' ' && board[1] == board[5] && board[1] == board[9])
        return (board[1] == 'X') ? 1 : 2; // Player 1 or Player 2 wins
    if (board[3] != ' ' && board[3] == board[5] && board[3] == board[7])
        return (board[3] == 'X') ? 1 : 2; // Player 1 or Player 2 wins

    return 0; // No winner
}

void play_game(int shmid, int semid, int player, int sockfd1, int sockfd2)
{

    char *board = shmat(shmid, NULL, 0);

    int n;
    struct sembuf sb;
    sb.sem_flg = 0;

    while (1)
    {
        printf("Game Started\n");
        int winner = 0;
        // Wait for the current player's turn

        sb.sem_num = (player == 1) ? 0 : 1;
        sb.sem_op = SEM_UNDO; // Wait (decrement) until it's the current player's turn
        semop(semid, &sb, 1);

        // Once it's the current player's turn, display the current board
        printf("Current Board:\n%s\n", board + 1);

        // Send the current board state to the appropriate client
        const char *turn_message = "Your turn.";
        // Send board state and turn message separately with a delimiter
        char message[512]; // Adjust buffer size as needed
        sprintf(message, "%s|%s", board, turn_message);

        if (player == 1)
        {
            printf("Sending board state to Player 1.\n");
            send(sockfd1, message, strlen(message), 0);
        }
        else
        {
            printf("Sending board state to Player 2.\n");
            send(sockfd2, message, strlen(message), 0);
        }

        // Receive move from the current player
        printf("Player %d's turn. Waiting for move...\n", player);
        char buffer[256];
        bzero(buffer, 256);
        if (player == 1)
        {
            read(sockfd1, buffer, 255);
            printf("Move received by player 1 %s\n", buffer);
        }
        else
        {
            read(sockfd2, buffer, 255);
            printf("Move received by player 2 %s\n", buffer);
        }

        n = atoi(buffer);

        // Validate the move
        if (n < 1 || n > 9 || board[n] != ' ')
        {
            printf("Invalid move! Please try again.\n");

            // Release the semaphore to allow the other player to take a turn
            sb.sem_op = 1;
            semop(semid, &sb, 1);

            continue;
        }

        // Update the board with the current player's move
        board[n] = (player == 1) ? 'X' : 'O';

        // Check for a winner
        winner = check_winner(board);
        if (winner) {
    	if (winner == 0) {
        printf("Draw");
    	} else {
        printf("Player %d wins!\n", winner);
        char win_message[20];
        char lose_message[20];
        sprintf(win_message, "You win");
        sprintf(lose_message, "You lose");

        // Send winning message to the winner
        if (winner == 1) {
            send(sockfd1, win_message, strlen(win_message), 0);
            send(sockfd2, lose_message, strlen(lose_message), 0);
        } else {
            send(sockfd2, win_message, strlen(win_message), 0);
            send(sockfd1, lose_message, strlen(lose_message), 0);
        }
    	}
            // Release the semaphore to allow the other player to take a turn (even though the game is over)
            sb.sem_op = 1;
            semop(semid, &sb, 1);
            return; // Exit the game loop if there's a winner
        }
        player = (player == 1) ? 2 : 1;
        // Signal other player's turn
        if (player == 1)
        {
            sb.sem_num = 1;
            sb.sem_op = 1;
        }
        else
        {
            sb.sem_num = 0;
            sb.sem_op = 1;
        }
        semop(semid, &sb, 1);
    }
}

int main()
{
    int sockfd, newsockfd1, newsockfd2, clilen, shmid, semid;
    struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
    char *board;

    // Create shared memory segment
    key_t key = ftok("shmfile", 65);
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0)
        error("ERROR creating shared memory");

    // Attach shared memory segment
    board = (char *)shmat(shmid, NULL, 0);
    if (board == (char *)-1)
        error("ERROR attaching shared memory");

    // Initialize shared memory
    memset(board, ' ', SHMSZ);
    board[0] = 'N'; // No winner yet

    // Create semaphores
    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 0); // Initialize semaphore 1 to 0
    semctl(semid, 1, SETVAL, 0); // Initialize semaphore 2 to 0

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Setup server address struct
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Listen for incoming connections
    listen(sockfd, 2);
    clilen = sizeof(cli_addr1);

    while (1)
    {
        // Accept connection from first player
        newsockfd1 = accept(sockfd, (struct sockaddr *)&cli_addr1, &clilen);
        if (newsockfd1 < 0)
            error("ERROR on accept from player 1");
        printf("Player 1 connected\n");

        // Set player number based on the order of connection
        int player = 1;

        // Accept connection from second player
        clilen = sizeof(cli_addr2);
        newsockfd2 = accept(sockfd, (struct sockaddr *)&cli_addr2, &clilen);
        if (newsockfd2 < 0)
            error("ERROR on accept from player 2");
        printf("Player 2 connected\n");

        // If socket 2 connected first, swap player numbers
        // If socket 2 connected first, swap player numbers
        if (newsockfd2 < newsockfd1)
        {
            player = 2;
            if (fork() == 0)
            {
                printf("Child process created for Player 2\n");
                close(sockfd); // Close server socket in child process
                printf("Child process executing play_game\n");
                play_game(shmid, semid, player, newsockfd2, newsockfd1);
                printf("Child process exited play_game\n");
                exit(0); // Exit child process after the game is finished
            }
        }
        else
        {
            if (fork() == 0)
            {
                printf("Child process created for Player 1\n");
                close(sockfd); // Close server socket in child process
                printf("Child process executing play_game\n");
                play_game(shmid, semid, player, newsockfd1, newsockfd2);
                printf("Child process exited play_game\n");
                exit(0); // Exit child process after the game is finished
            }
        }

        // Close sockets in parent process
        close(newsockfd1);
        close(newsockfd2);
    }

    // Clean up
    shmdt(board);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, 0);

    return 0;
}