#ifndef SERVER_UTILS
#define SERVER_UTILS

#include "../utils.h"
#include "client_context.h"
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct Command
{
    int client_socket;
    int version;
    int function;
    String name;
    String password;
    client_message message;
} Command;

void setup_db(void);
void setup_sig_handle();
void setup_socket();
void signal_handler(int sig);
void setup_thread_pool();
void idle_task();
void *handle_client(void *arg);

String execute_command(int client_socket, char *client_message, error_message *error);

extern int server_socket;
extern struct sockaddr_in server;
extern pthread_t *threads;

#endif // SERVER_UTILS

// zanadu: original hyper text '
// a statless object  means a proccedure
// statless bisness obejcts means

// Are firewalls an operating system level programs
// or a hardware level sysetm

// backend f a webserver tcp/ip