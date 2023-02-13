#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

int socket_fd;

void child_task(int socket_desc)
{
    int n;
    char buffer[256];
    while (1)
    {
        n = read(socket_desc, buffer, 255);
        if (n < 0)
        {
            perror("Error reading from socket");
            break;
        }
        if (n == 0)
        {
            printf("Connection closed by parent\n");
            break;
        }
        buffer[n] = '\0';
        printf("Received message: %s\n", buffer);
    }
}

void create_child(void)
{
    char *term = getenv("TERM");
    if (!term)
    {
        fprintf(stderr, "Error: TERM environmental variable is not set\n");
        exit(1);
    }

    int pid = fork();
    if (pid == 0)
    {
        // Child process
        execlp(term, term, "-x", "./child", NULL);
    }
    else if (pid > 0)
    {
        // Parent process
        printf("Child process created with PID %d\n", pid);
    }
    else
    {
        perror("Error creating child process");
        exit(1);
    }
}

void send_message(const char *message)
{
    if (send(socket_fd, message, strlen(message), 0) == -1)
    {
        perror("Error sending message");
        exit(1);
    }
}

void check_status(void)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        printf("Child process with PID %d has terminated\n", pid);
        exit(0);
    }
}

void close_child(void)
{
    send_message("close");
}

int create_internal_socket()
{
    int sock_desc;
    struct sockaddr_in serv_addr;

    // Create socket
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize socket address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(0);

    // Bind the socket to a local address
    if (bind(sock_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket");
        exit(1);
    }

    // Return the socket descriptor
    return sock_desc;
}

int main(int argc, char *argv[])
{
    create_child();
    create_internal_socket();

    // Send message to child process
    send_message("Hello from parent!");

    // Check status of child process
    check_status();

    // Close child process
    close_child();

    return 0;
}
