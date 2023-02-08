#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>

#include "server_utils.h"
#include "data_structures.h"
#include "client_context.h"

void list_accounts(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)cl_msg;
    (void)client_socket;
    (void)err;

    char *users_string = malloc(INPUT_SIZE);
    int printed = 0;
    for (size_t i = 0; i < users->size; i++)
    {
        node *curr = users->array[i];
        while (curr)
        {
            printed += sprintf(users_string + printed, "=> %s\n", (char *)curr->key);
            curr = curr->next;
        }
    }
    users_string[printed] = '\0';
    msg->s = users_string;
    msg->len = printed;
}

void create_account(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)msg;
    (void)client_socket;

    int *msg_int = (int *)cl_msg;
    int name_len = msg_int[2];
    int password_len;
    memcpy(&password_len, cl_msg + (3 * sizeof(int)) + name_len, sizeof(int));
    char *name = malloc((name_len + 1) * sizeof(char));
    char *password = malloc((password_len + 1) * sizeof(char));

    memcpy(name, cl_msg + (3 * sizeof(int)), name_len);
    memcpy(password, cl_msg + (3 * sizeof(int)) + name_len + sizeof(int), password_len);

    name[name_len] = '\0';
    password[password_len] = '\0';

    // check if user exists
    hash_elements result = get_ht(users, name);
    if (result.values_size > 0)
    {
        // user already exists
        err->error_code = USER_NAME_EXISTS;
        err->error_message = USER_NAME_EXISTS_MSG;
        return;
    }

    User *new_user = malloc(sizeof(User));
    new_user->name = (String){
        .s = name,
        .len = name_len};
    new_user->password = (String){
        .s = password,
        .len = password_len};
    new_user->user_logged_in = false;
    new_user->client_socket = -1;

    // add user to users hashtable
    put_ht(users, name, new_user);
}

String formate_messages(User *user)
{
    char *msg = malloc(sizeof(char) * INPUT_SIZE);
    int msg_len = 0;

    linked_list *curr = user->recieved_messgs;
    linked_list *prev = NULL;
    while (curr != NULL)
    {
        client_message *m = (client_message *)curr->data;
        memcpy(msg + msg_len, m->sender.s, m->sender.len);
        msg_len += m->sender.len;
        msg[msg_len] = ':';
        msg_len++;
        memcpy(msg + msg_len, m->msg.s, m->msg.len);
        msg_len += m->msg.len;
        msg[msg_len] = ';';
        msg_len++;

        prev = curr;
        curr = curr->next;

        free(m->sender.s);
        free(m->msg.s);
        free(m);
        free(prev);
    }

    user->recieved_messgs = NULL;
    msg[msg_len] = '\0';
    return (String){
        .s = msg,
        .len = msg_len};
}

void receive_message(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)msg;
    (void)cl_msg;

    // check if user is logged in
    hash_elements result = get_ht(socket_map, &client_socket);
    if (result.values_size == 0)
    {
        // user is not logged in
        err->error_code = USER_NOT_LOGGED_IN;
        err->error_message = USER_NOT_LOGGED_IN_MSG;
        return;
    }

    User *user = result.values[0];
    *msg = formate_messages(user);
}

void login_account(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)msg;
    (void)client_socket;

    int *msg_int = (int *)cl_msg;
    int name_len = msg_int[2];
    int password_len;
    memcpy(&password_len, cl_msg + (3 * sizeof(int)) + name_len, sizeof(int));
    char *name = malloc((name_len + 1) * sizeof(char));
    char *password = malloc((password_len + 1) * sizeof(char));

    memcpy(name, cl_msg + (3 * sizeof(int)), name_len);
    memcpy(password, cl_msg + (3 * sizeof(int)) + name_len + sizeof(int), password_len);

    name[name_len] = '\0';
    password[password_len] = '\0';

    // check if user exists
    hash_elements result = get_ht(users, name);
    if (result.values_size == 0)
    {
        // user does not exist
        err->error_code = USER_DOESNT_EXIST;
        err->error_message = USER_DOESNT_EXIST_MSG;
        return;
    }

    User *user = result.values[0];
    if (user->user_logged_in)
    {
        // user already logged in
        err->error_code = USER_ALREADY_LOGGED_IN;
        err->error_message = USER_ALREADY_LOGGED_IN_MSG;
        return;
    }

    if (strcmp(user->password.s, password) != 0)
    {
        // wrong password
        err->error_code = INCORRECT_PASSWORD;
        err->error_message = INCORRECT_PASSWORD_MSG;
        return;
    }

    // login user
    user->user_logged_in = true;
    // store the client socket
    user->client_socket = client_socket;

    // add user to socket hashtable
    put_ht(socket_map, &client_socket, user);

    // send the user messages if there are any
    if (user->recieved_messgs != NULL)
    {
        *msg = formate_messages(user);
    }
}

void send_message(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)msg;

    // check if this user is logged in
    hash_elements result = get_ht(socket_map, &client_socket);
    if (result.values_size == 0)
    {
        // user is not logged in
        err->error_code = USER_NOT_LOGGED_IN;
        err->error_message = USER_NOT_LOGGED_IN_MSG;
        return;
    }
    User *sender = result.values[0];

    int *msg_int = (int *)cl_msg;
    int name_len = msg_int[2];
    int message_len;
    memcpy(&message_len, cl_msg + (3 * sizeof(int)) + name_len, sizeof(int));
    char *name = malloc((name_len + 1) * sizeof(char));
    char *message = malloc((message_len + 1) * sizeof(char));

    memcpy(name, cl_msg + (3 * sizeof(int)), name_len);
    memcpy(message, cl_msg + (3 * sizeof(int)) + name_len + sizeof(int), message_len);

    name[name_len] = '\0';
    message[message_len] = '\0';

    // check if user exists
    result = get_ht(users, name);
    if (result.values_size == 0)
    {
        // user does not exist
        err->error_code = RECEIVER_DOESNT_EXIST;
        err->error_message = RECEIVER_DOESNT_EXIST_MSG;
        return;
    }

    // add the message to the receiver's message queue
    User *reciever = result.values[0];

    if (reciever->recieved_messgs)
    {
        reciever->recieved_messgs = malloc(sizeof(linked_list));
    }
    else
    {
        linked_list *new_node = malloc(sizeof(linked_list));
        new_node->next = reciever->recieved_messgs;
        reciever->recieved_messgs = new_node;
    }

    client_message *new_message = malloc(sizeof(client_message));
    new_message->sender = (String){
        .s = sender->name.s,
        .len = sender->name.len};
    new_message->msg = (String){
        .s = message,
        .len = message_len};
    reciever->recieved_messgs->data = new_message;
}

void delete_account(int client_socket, char *cl_msg, String *msg, error_message *err)
{
    (void)msg;
    (void)cl_msg;

    // check if this user is logged in
    hash_elements result = get_ht(socket_map, &client_socket);
    if (result.values_size == 0)
    {
        // user is not logged in
        err->error_code = USER_NOT_LOGGED_IN;
        err->error_message = USER_NOT_LOGGED_IN_MSG;
        return;
    }
    User *user = result.values[0];

    // delete the user from the users hashtable
    erase_ht(users, user->name.s);

    // delete the user from the socket hashtable
    erase_ht(socket_map, &client_socket);

    // free the user
}

String execute_command(int client_socket, char *cl_msg, error_message *err)
{

    // decode the incoming byte array according to the protocol as defined
    // in the specification document and execute the command
    // return the result as a string

    String return_msg = {
        .len = 2,
        .s = "OK"};

    int *msg_int = (int *)cl_msg;
    int version = msg_int[0];
    int function = msg_int[1];

    // check version
    if (version != JER_HI_VERSION)
    {
        err->error_code = INVALID_VERSION;
        err->error_message = INVALID_VERSION_MSG;
        return return_msg;
    }

    switch (function)
    {
    case CREATE_ACCOUNT:
        create_account(client_socket, cl_msg, &return_msg, err);
        break;
    case LOGIN:
        login_account(client_socket, cl_msg, &return_msg, err);
        break;
    case LIST_ACCOUNTS:
        list_accounts(client_socket, cl_msg, &return_msg, err);
        break;
    case SEND_MESSAGE:
        send_message(client_socket, cl_msg, &return_msg, err);
        break;
    case RECEIVE_MESSAGE:
        receive_message(client_socket, cl_msg, &return_msg, err);
        break;
    case DELETE_ACCOUNT:
        delete_account(client_socket, cl_msg, &return_msg, err);
        break;
    default:
        break;
    }
    return return_msg;
}
