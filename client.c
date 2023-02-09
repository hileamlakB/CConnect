#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "utils.h"
#include "client_utils.h"

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
    server.sin_port = htons(8888);

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect failed");
        exit(1);
    }

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
            printf("Error: %s", error.error_message);
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

        if (strcmp(message, "OK"))
        {
            printf("%s\n", message);
        }

    }

    // Close socket
    close(socket_desc);
    return 0;
}
