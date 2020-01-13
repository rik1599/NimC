#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>

#include "server.h"
#include "tools.h"

int createAndBind(int domain, int type, int protocol, struct sockaddr *address, int limit)
{
    int sock = socket(domain, type, protocol);
    checkWithExit(sock, 1, "socket()");
    checkWithExit(bind(sock, address, sizeof address), 2, "bind()");
    listen(sock, limit);
    fprintf(stderr, "In ascolto.\n");
    return sock;
}

int connectToPlayer(int socket)
{
    struct sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Attesa indefinita di una connessione, se fallisce ne cerca un'altra
    while (1)
    {
        int fd = accept(socket, (struct sockaddr *)&client_addr, &client_len);
        if (fd == -1)
        {
            fprintf(stderr, "accept()");
        }
        else
        {
            sendMessage(fd, "Connesso!, In attesa di un altro giocatore...");
            return fd;
        }
    }
}

game_t *iniziaPartita(int playerOne, int playerTwo, field_t *field)
{
    game_t *newGame = calloc(1, sizeof(game_t));

    newGame->field = field;
    newGame->players[0] = playerOne;
    newGame->players[1] = playerTwo;
    newGame->turn = 0;

    return newGame;
}

void *playGame(void *arg)
{
    game_t *game = (game_t *) arg;
    for (size_t i = 0; i < 2; i++)
    {
        sendMessage(game->players[i], "Partita iniziata");
        if (game->turn == i)
        {
            sendMessage(game->players[i], "Comincia tu!");
        }
        
    }
    
}

int sendMessage(int player, char *message)
{

}

void turno(game_t *game)
{
}

void terminaPartita(int player, int code, game_t *game)
{
}