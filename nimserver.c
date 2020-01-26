#include <stdlib.h>
#include <signal.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "server.h"
#include "game.h"
#include "tools.h"

#define PENDING_PLAYERS 10

int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    unlink(SOCKADDR);

    struct sockaddr_un addr = {
        .sun_family = AF_LOCAL,
        .sun_path = SOCKADDR
    };

    int sock = createAndBind(AF_LOCAL, SOCK_STREAM, 0, (struct sockaddr *) &addr, sizeof(addr), PENDING_PLAYERS);
    
    while (1)
    {   
        int player1 = connectToPlayer(sock);
        int player2 = connectToPlayer(sock);
        field_t *field = createField();
        game_t *game = iniziaPartita(player1, player2, field);

        //Passiamo la partita su un altro thread
        pthread_t thread;
        pthread_create(&thread, NULL, playGame, game);
        pthread_detach(thread);
    }

    return 0;
}
