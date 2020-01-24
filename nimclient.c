#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "game.h"
#include "client.h"
#include "tools.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_un address = {AF_LOCAL, SOCKADDR};
    int server = connectToServer(AF_LOCAL, SOCK_STREAM, 0, (struct sockaddr *) &address, sizeof(address));

    field_t* field = startGame(server);
    int esito = turno(field, server, 1);
    while (esito == 2)
    {
        esito = turno(field, server, 0);
    }

    receiveMessage(server, stdout);
    
    close(server);
    free(field);

    return 0;
}
