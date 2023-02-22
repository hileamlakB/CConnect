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

int socket_desc;

void *recieve_thread(void *args)
{
    (void)args;
    error_message er;
    String parse_cmd = parse_receive(NULL, &er);
    char *message = malloc(INPUT_SIZE * sizeof(char));

    while (true)
    {
        // send message to socket
        // if recives message display and yield for 2 seconds
        if (send(socket_desc, parse_cmd.s, parse_cmd.len, 0) < 0)
        {
            perror("Send failed");
            exit(SOCKET_FAILURE);
        }
        if (recv(socket_desc, message, INPUT_SIZE, 0) < 0)
        {
            perror("Receive failed");
            exit(SOCKET_FAILURE);
        }
        if (strncmp(message, "Received Messages", 17) == 0)
        {

            int msg_len = ((int *)message)[0];
            message[msg_len + sizeof(int)] = '\0';
            printf("%s\n", message + sizeof(int));
        }
    }
}

void setup_receiver()
{
    pthread_t reciever_t;
    pthread_create(&reciever_t, NULL, recieve_thread, NULL);
}

int main(void)
{
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
    // server.sin_addr.s_addr = inet_addr("68.183.26.103");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(2625);

    // Connect to server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect failed");
        exit(1);
    }

    setup_receiver();
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

        if (strcmp(message, "OK") == 0)
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
