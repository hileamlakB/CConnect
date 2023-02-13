#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server_utils.h"
#include "data_structures.h"
#include "client_context.h"

hashtable *users = NULL;
hashtable *socket_map = NULL;
int server_socket;
struct sockaddr_in server;
pthread_t *threads;

size_t hash_user(hash_element user, size_t n)
{
    return hash_string((char *)user, n);
}

long robert_junkins_hash(long a)
{
    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) ^ (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a + 0xd3a2646c) ^ (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) ^ (a >> 16);
    return a;
}

size_t int_hash(hash_element element, size_t size)
{
    return robert_junkins_hash(*((int *)element)) % size;
}

size_t cmp_sockets(hash_element socket1, hash_element socket2)
{
    return *((int *)socket1) - *((int *)socket2);
}

size_t cmp_user(hash_element user1, hash_element user2)
{
    return strcmp((char *)user1, (char *)user2);
}

void free_none(hash_element user)
{
    (void)user;
    // free a user lement
    // pass for now
}

// free_none is to be implemented
void setup_db(void)
{
    create_ht(&users, HASH_SIZE, hash_user, cmp_user, false, free, free_none);
    create_ht(&socket_map, HASH_SIZE, int_hash, cmp_sockets, false, free_none, free_none);
}

void idle_task()
{
    while (true)
    {
        sched_yield();
    }
}

void signal_handler(int sig)
{
    // write error to log file
    // close all sockets

    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1)
    {
        perror("Could not open log file");
        exit(1);
    }

    // Do more better logging and
    // cleanup including storing data
    switch (sig)
    {
    case SIGINT:
        ssize_t err = write(fd, "Caught SIGINT!\n", 15);
        if (err < -1) {
            exit(err);
        }
        break;
    case SIGSEGV:
        ssize_t err = write(fd, "Caught SIGSEGV!\n", 16);
        if (err < -1) {
            exit(err);
        }
        break;
    default:
        ssize_t err = write(fd, "Caught unhandled signal!\n", 26);
        if (err < -1) {
            exit(err);
        }
        break;
    }

    close(server_socket);
    exit(1);
}

void setup_sig_handle()
{
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
}

void setup_socket()
{

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Could not create socket");
        exit(1);
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed");
        exit(1);
    }

    // Listen
    listen(server_socket, MAX_CLIENTS);
    printf("Waiting for incoming connections...\n");
}

void setup_thread_pool()
{
    threads = (pthread_t *)malloc(sizeof(pthread_t) * NUM_THREADS);
    // create thread pool
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, handle_client, NULL);
    }
}

// change user login status to offline once the client disconnects
// or another login happens on the same client
// or prevent multiple logins on the same client