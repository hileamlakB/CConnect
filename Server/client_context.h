#ifndef CLIENT_CONTEXT_H
#define CLIENT_CONTEXT_H

#include "../utils.h"
#include "data_structures.h"

#define HASH_SIZE 101

typedef struct client_message
{
    String msg;
    String sender;
    String reciever;
} client_message;

typedef struct User
{
    bool user_logged_in;
    int client_socket;
    String name;
    String password;
    linked_list *sent_messgs;
    linked_list *recieved_messgs;
} User;

extern hashtable *users;
extern hashtable *socket_map;

#endif // CLIENT_CONTEXT_H
