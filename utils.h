#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

#define INPUT_SIZE 4096

#define MAX_CLIENTS 100
#define NUM_THREADS 10

typedef struct String
{
    char *s;
    size_t len;
} String;

typedef struct message_header
{
    int version_no;
    int func_id;
} message_header;

#define JER_HI_VERSION 0
#define VERSION_SIZE 4
#define FUNCTION_SIZE 4
#define STRING_LEN_SIZE 4

//  function codes
#define CREATE_ACCOUNT 1
#define LOGIN 2
#define LIST_ACCOUNTS 3
#define SEND_MESSAGE 4
#define RECEIVE_MESSAGE 5
#define DELETE_ACCOUNT 6

// error codes
#define SUCCESS 0
#define INVALID_FUNCTION 1
#define INVALID_ARGUMENTS 2
#define INVALID_USERNAME 3
#define INVALID_PASSWORD 4
#define INVALID_MESSAGE 5
#define INVALID_ACCOUNT 6
#define INVALID_VISION 7

#define INVALID_FUNCTION_MSG "Invalid function"
#define INVALID_ARGUMENTS_MSG "Invalid arguments"
#define INVALID_USERNAME_MSG "Invalid username"
#define INVALID_PASSWORD_MSG "Invalid password"
#define INVALID_MESSAGE_MSG "Invalid message"
#define INVALID_ACCOUNT_MSG "Invalid account"

// server errors
#define USER_NAME_EXISTS 8
#define USER_DOESNT_EXIST 9
#define INCORRECT_PASSWORD 10
#define NON_EXISTING_USER_CANT_BE_DELETED 11
#define INVALID_VERSION 12
#define USER_ALREADY_LOGGED_IN 13
#define USER_NOT_LOGGED_IN 14
#define RECEIVER_DOESNT_EXIST 15
#define MULTIPLE_USERS_ON_SAME_SOCKET 16

#define USER_NAME_EXISTS_MSG "USER NAME ALREADY EXISTS"
#define USER_DOESNT_EXIST_MSG "USER DOESN'T EXIST"
#define INCORRECT_PASSWORD_MSG "INCORRECT PASSWORD"
#define NON_EXISTING_USER_CANT_BE_DELETED_MSG "NON EXISTING USER CANT BE DELETED"
#define INVALID_VERSION_MSG "UNSUPORTED VERSION"
#define USER_ALREADY_LOGGED_IN_MSG "USER ALREADY LOGGED IN"
#define USER_NOT_LOGGED_IN_MSG "USER NOT LOGGED IN"
#define RECEIVER_DOESNT_EXIST_MSG "RECEIVER DOESN'T EXIST"
#define MULTIPLE_USERS_ON_SAME_SOCKET_MSG "ONLY ONE USER PER SOCKET ALLOWED"

typedef struct error_message
{
    int error_code;
    char *error_message;
} error_message;

#endif // UTILS_H
