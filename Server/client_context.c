#include <string.h>
#include <stdio.h>

#include "client_context.h"
#include "data_structures.h"
#include "server_utils.h"
#include "../utils.h"

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

            error_message err = (error_message){
                .error_code = 0,
                .error_message = "OK",
            };
            String res = execute_command(client_socket, client_message, &err);

            // send response
            if (err.error_code != 0)
            {
                int msg_len = strlen(err.error_message);
                char *msg = malloc(msg_len + sizeof(int) + 1);
                memcpy(msg, &msg_len, sizeof(int));
                memcpy(msg + sizeof(int), err.error_message, msg_len);
                msg[msg_len + sizeof(int)] = '\0';

                send(client_socket, msg, msg_len + sizeof(int) + 1, 0);
                free(msg);
            }
            else
            {
                printf("sennding %s\n", res.s);
                char *msg = malloc(res.len + sizeof(int) + 1);
                memcpy(msg, &res.len, sizeof(int));
                memcpy(msg + sizeof(int), res.s, res.len);
                msg[res.len + sizeof(int)] = '\0';
                send(client_socket, msg, res.len + sizeof(int) + 1, 0);
                free(msg);
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
