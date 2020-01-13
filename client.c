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

int turno(field_t *field) 
{
    
}

