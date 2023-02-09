#include <string.h>
#include <stdio.h>

#include "client_context.h"
#include "data_structures.h"
#include "server_utils.h"
#include "utils.h"

void *handle_client(void *arg)
{
    (void)arg;

    while (true)
    {
        int client_socket;
        struct sockaddr_in client;
        int c = sizeof(struct sockaddr_in);

        char client_message[INPUT_SIZE];
        int read_size;

        client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&c);
        if (client_socket < 0)
        {
            perror("Accept failed");
            exit(1);
        }

        printf("Connection accepted %li\n", pthread_self());

        // Receive message from client
        while ((read_size = recv(client_socket, client_message, INPUT_SIZE, 0)) > 0)
        {
            client_message[read_size] = '\0';

            error_message err;
            String res = execute_command(client_socket, client_message, &err);

            // send response
            if (err.error_code != 0)
            {
                send(client_socket, err.error_message, strlen(err.error_message), 0);
            }
            else
            {
                printf("sennding %s\n", res.s);
                send(client_socket, res.s, res.len, 0);
            }
        }

        if (read_size == 0)
        {
            printf("Client disconnected\n");
            fflush(stdout);
        }
        else if (read_size == -1)
        {
            perror("recv failed");
        }
        // accept
        // execute
        // repeat
        // remove from client hash table
        // set the status of user to logged out if they were logged int
        hash_elements result = get_ht(socket_map, (hash_element)&client_socket);
        if (result.values_size > 0)
        {
            User *u = result.values[0];
            u->user_logged_in = false;
        }
        erase_ht(socket_map, (hash_element)&client_socket);
    }

    return NULL;
}
