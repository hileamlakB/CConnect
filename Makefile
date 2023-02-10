all: server_build client_build

# C-compiler settings
CC = gcc -std=c99 -g -ggdb3

# Default optimization level
O ?= 0

# Flags and other libraries
override CFLAGS += -Wall -Wextra -pedantic -pthread -O$(O) -I$(INCLUDES)
LDFLAGS =
LIBS =
INCLUDES = include

client_build: Client/client.o Client/client_utils.o 
	$(CC) $(CFLAGS) $(DEPCFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

server_build: Server/server.o Server/client_context.o Server/server_utils.c Server/hash_table.c Server/setup.o
	$(CC) $(CFLAGS) $(DEPCFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f client_build server_build *.o *~
	rm -rf .deps

distclean: clean
	rm -rf $(DEPSDIR)

.PHONY: all clean distclean