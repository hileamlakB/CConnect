#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include "../utils.h"
#define SOCKET_FAILURE 10
#define EXIT -2
#define HELP -1
#define HELP_MSG "Jeremy Is a socket based chat room. \n You can create an account, login, send messages, receive messages, and delete your account. \n To get more information on a specific command, type 'help <command>'. \n Commands: \n create \n list \n login \n send \n receive \n delete \n help \n exit \n"
#define HELP_CREATE "create <username> <password> \n \t Creates a new account with the given username and password. \n"
#define HELP_LIST "list \n \t Lists all users currently logged in. \n"
#define HELP_LOGIN "login <username> <password> \n \t Logs in to an existing account with the given username and password. \n"
#define HELP_SEND "send <username> <message> \n \t Sends a message to the given user. \n"
#define HELP_RECEIVE "receive \n \t Receives all messages sent to you. \n"
#define HELP_DELETE "delete \n \t Deletes your account. \n"
#define HELP_EXIT "exit \n \t Exits the program. \n"
#define HELP_HELP "help \n \t Displays this help message. \n"

#define LOGIN_MSG "LOGING SUCCESSFUL"
#define SENT_MSG "SENT"
#define RECEIVED_MSG "RECEIVED"
#define DELETED_MSG "DELETED"
#define CREATED_MSG "ACCOUNT CREATED"

String parse_create(char *message, error_message *error);
String parse_list(char *message, error_message *error);
String parse_login(char *message, error_message *error);
String parse_send(char *message, error_message *error);
String parse_receive(char *message, error_message *error);
String parse_delete(char *message, error_message *error);
String parse_input(char *message, error_message *error);

#endif // CLIENT_UTILS_H
