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

    receiveMessage(sock);

    return sock;
}

void receiveMessage(int server)
{
    char *msg = malloc(sizeof(char));
    checkWithExit(
        receive(server, msg, sizeof(char)),
        -3,
        "Receive message failed"
    );
    printf(msg);
    printf("\n");
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

