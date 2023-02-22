#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "../utils.h"
#include "client_utils.h"
#include <pthread.h>

int main(void)
{
    int socket_desc;
    struct sockaddr_in server;
    char *message = malloc(INPUT_SIZE * sizeof(char));

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket");
        exit(1);
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("68.183.26.103");
    // server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(2625);

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect failed");
        exit(1);
    }

    if (!isatty(STDIN_FILENO))
    {
        // get the commands one line at a time
        // send the command to the server like the normal client does
        // print the response from the server
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, stdin)) != -1)
        {
            line[strlen(line) - 1] = '\0';
            // printf("Retrieved line of length %s:", line);
            error_message error = {0, NULL};
            String cmd = parse_input(line, &error);

            if (error.error_code == HELP)
            {
                printf("%s", error.error_message);
                continue;
            }

            if (error.error_code == EXIT)
            {
                printf("Graceful Exit\n");
                break;
            }

            if (error.error_code != 0)
            {
                printf("Error: %s\n", error.error_message);
                continue;
            }

            if (send(socket_desc, cmd.s, cmd.len, 0) < 0)
            {
                perror("Send failed");
                exit(SOCKET_FAILURE);
            }

            if (recv(socket_desc, message, INPUT_SIZE, 0) < 0)
            {
                perror("Receive failed");
                exit(SOCKET_FAILURE);
            }

            int msg_len = ((int *)message)[0];
            message[msg_len + sizeof(int)] = '\0';
            printf("%s\n", message + sizeof(int));
        }

        return 0;
    }

    // setup_receiver();
    // printf("Connected to server\n");
    printf("Welcome to the chat room! For a list of commands type help\n");

    // Get user input, send to server, receive reply, print reply
    // and repeat until user exits
    while (true)
    {
        printf(">  ");
        fgets(message, INPUT_SIZE, stdin);
        message[strlen(message) - 1] = '\0';

        error_message error = {0, NULL};
        String cmd = parse_input(message, &error);

        // print the help message to the console
        if (error.error_code == HELP)
        {
            printf("%s", error.error_message);
            continue;
        }

        // exit the terminal
        if (error.error_code == EXIT)
        {
            printf("Graceful Exit\n");
            break;
        }

        // error
        if (error.error_code != 0)
        {
            printf("Error: %s\n", error.error_message);
            continue;
        }

        // failed to send
        if (send(socket_desc, cmd.s, cmd.len, 0) < 0)
        {
            perror("Send failed");
            exit(SOCKET_FAILURE);
        }

        // failed to receive
        if (recv(socket_desc, message, INPUT_SIZE, 0) < 0)
        {
            perror("Receive failed");
            exit(SOCKET_FAILURE);
        }

        if (strcmp(message, "OK"))
        {
            int msg_len = ((int *)message)[0];
            message[msg_len + sizeof(int)] = '\0';
            printf("%s\n", message + sizeof(int));
        }
    }

    // Close socket
    close(socket_desc);
    return 0;
}
