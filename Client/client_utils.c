#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "../utils.h"
#include "client_utils.h"

String parse_create(char *message, error_message *error)
{
    int version = VERSION_NO;
    int func_id = CREATE_ACCOUNT;
    char *name = strsep(&message, " ");
    if (name == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }

    char *password = strsep(&message, " ");
    if (password == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }
    size_t name_len = strlen(name);
    size_t pass_len = strlen(password);

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE + STRING_LEN_SIZE + name_len + STRING_LEN_SIZE + pass_len;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;
    memcpy(byte_array + copied, &name_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, name, name_len);
    copied += name_len;
    memcpy(byte_array + copied, &pass_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, password, pass_len);
    copied += pass_len;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_list(char *message, error_message *error)
{
    (void)message;
    (void)error;

    int version = VERSION_NO;
    int func_id = LIST_ACCOUNTS;

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_login(char *message, error_message *error)
{
    int version = VERSION_NO;
    int func_id = LOGIN;
    char *name = strsep(&message, " ");
    if (name == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }

    char *password = strsep(&message, " ");
    if (password == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }
    size_t name_len = strlen(name);
    size_t pass_len = strlen(password);

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE + STRING_LEN_SIZE + name_len + STRING_LEN_SIZE + pass_len;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;
    memcpy(byte_array + copied, &name_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, name, name_len);
    copied += name_len;
    memcpy(byte_array + copied, &pass_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, password, pass_len);
    copied += pass_len;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_send(char *message, error_message *error)
{
    int version = VERSION_NO;
    int func_id = SEND_MESSAGE;
    char *name = strsep(&message, " ");
    if (name == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }

    if (message == NULL)
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
        return (String){NULL, 0};
    }
    size_t name_len = strlen(name);
    size_t message_len = strlen(message);

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE + STRING_LEN_SIZE + name_len + STRING_LEN_SIZE + message_len;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;
    memcpy(byte_array + copied, &name_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, name, name_len);
    copied += name_len;
    memcpy(byte_array + copied, &message_len, STRING_LEN_SIZE);
    copied += STRING_LEN_SIZE;
    memcpy(byte_array + copied, message, message_len);
    copied += message_len;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_receive(char *message, error_message *error)
{
    (void)message;
    (void)error;
    int version = VERSION_NO;
    int func_id = RECEIVE_MESSAGE;

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_delete(char *message, error_message *error)
{
    (void)message;
    (void)error;

    int version = VERSION_NO;
    int func_id = DELETE_ACCOUNT;

    // create a byte array
    size_t byte_len = VERSION_SIZE + FUNCTION_SIZE;
    char *byte_array = malloc(byte_len * sizeof(char));

    size_t copied = 0;
    memcpy(byte_array + copied, &version, VERSION_SIZE);
    copied += VERSION_SIZE;
    memcpy(byte_array + copied, &func_id, FUNCTION_SIZE);
    copied += FUNCTION_SIZE;

    return (String){.len = byte_len, .s = byte_array};
}

String parse_help(char *message, error_message *error)
{
    (void)message;
    (void)error;

    error->error_code = HELP;
    error->error_message = HELP_MSG;

    char *help_type = strsep(&message, " ");
    if (help_type == NULL)
    {
        return (String){NULL, 0};
    }

    if (strcmp("create", help_type) == 0)
    {
        error->error_message = HELP_CREATE;
    }
    else if (strcmp("list", help_type) == 0)
    {
        error->error_message = HELP_LIST;
    }
    else if (strcmp("login", help_type) == 0)
    {
        error->error_message = HELP_LOGIN;
    }
    else if (strcmp("send", help_type) == 0)
    {
        error->error_message = HELP_SEND;
    }
    else if (strcmp("receive", help_type) == 0)
    {
        error->error_message = HELP_RECEIVE;
    }
    else if (strcmp("delete", help_type) == 0)
    {
        error->error_message = HELP_DELETE;
    }
    else if (strcmp("help", help_type) == 0)
    {
        error->error_message = HELP_HELP;
    }
    else if (strcmp("exit", help_type) == 0)
    {
        error->error_message = HELP_EXIT;
    }
    else
    {
        error->error_code = INVALID_ARGUMENTS;
        error->error_message = INVALID_ARGUMENTS_MSG;
    }
    return (String){NULL, 0};
}

String parse_input(char *message, error_message *error)
{
    char *token = strsep(&message, " ");
    // printf("token: %s\n", token);
    if (strncmp(token, "create", 6) == 0)
    {
        return parse_create(message, error);
    }
    else if (strncmp(token, "list", 4) == 0)
    {
        return parse_list(message, error);
    }
    else if (strncmp(token, "login", 5) == 0)
    {
        return parse_login(message, error);
    }
    else if (strncmp(token, "send", 4) == 0)
    {
        return parse_send(message, error);
    }
    else if (strncmp(token, "receive", 7) == 0)
    {
        return parse_receive(message, error);
    }
    else if (strncmp(token, "delete", 6) == 0)
    {
        return parse_delete(message, error);
    }
    else if (strncmp(token, "help", 4) == 0)
    {
        return parse_help(message, error);
    }
    else if (strncmp(token, "exit", 4) == 0)
    {
        error->error_code = EXIT;
        return (String){.len = 0, .s = NULL};
    }
    else
    {
        error->error_code = INVALID_FUNCTION;
        error->error_message = INVALID_FUNCTION_MSG;
        return (String){.len = 0, .s = NULL};
    }
}
