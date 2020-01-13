#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "tools.h"

int connectToServer(int domain, int type, int protocol, struct sockaddr *address)
{
    int sock = socket(domain, type, protocol);
    checkWithExit(sock, -1, "socket()");

    checkWithExit(
        connect(sock, address, sizeof(address)),
        -2,
        "Connection failed"
    );

    printf(receiveMessage(sock));
    printf("\n");

    return sock;
}

char *receiveMessage(int server)
{
    char *msg = malloc(sizeof(char));
    receive(server, msg, sizeof(char));
}

int turno(field_t *field) 
{
    
}

field_t *startGame(int server)
{
    field_t *field = malloc(sizeof(field_t));
    checkWithExit(
        receive(server, field, sizeof(field_t)),
        -2,
        "field_t"
    );
}

