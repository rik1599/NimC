#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    checkWithDisconnectAndExit(
        receive(server, msg, sizeof(char)),
        server,
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
    receiveMessage(server);

    int turn = 0;
    checkWithDisconnectAndExit(
        receive(server, &turn, sizeof(turn)),
        server,
        -2,
        "turn"
    );

    field_t *field = malloc(sizeof(field_t));
    checkWithDisconnectAndExit(
        receive(server, field, sizeof(field_t)),
        server,
        -2,
        "field_t"
    );

    if (turn == 0)
    {
        receiveMessage(server);
    }

    return field;
    
}

void checkWithDisconnectAndExit(int result, int server, int exitval, const char* msg)
{
    if (result == -1)
    {
        close(server);
        checkWithExit(result, exitval, msg);
    }
    
}

