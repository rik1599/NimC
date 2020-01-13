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
    int sock = connectToServer(AF_LOCAL, SOCK_STREAM, 0, (struct sockaddr *) &address);

    

    return 0;
}
