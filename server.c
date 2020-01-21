#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

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
        if (checkNoExit(fd, "accept()") != -1)
        {
            fd = checkAndDisconnect(sendMessage(fd, "Connesso!, In attesa di un altro giocatore..."), fd);

            if (fd != -1)
            {
                return fd;
            }
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
    game_t *game = (game_t *)arg;
    for (size_t i = 0; i < (sizeof(game->players) / sizeof(int)); i++)
    {
        sendSock(game->players[i], game->turn, sizeof(int));
        sendSock(game->players[i], game->field, sizeof(*game->field));
    }

    int winPlayer = winner(game);
    while (winPlayer == 2)
    {
        turno(game);
        cambiaTurno(game);
        winPlayer = winner(game);

        sendCode(game->players[game->turn], winPlayer);

        if (winPlayer == 2)
        {
            checkAndDisconnect(sendSock(game->players[game->turn], game->field, sizeof(field_t)), -2);
        }
    }

    terminaPartita(winPlayer, 0, game);
}

int sendMessage(int player, char *message)
{
    int size = (strlen(message) * sizeof(char));

    sendSock(player, message, size);
}

int turno(game_t *game)
{
    int pila;
    int numeroDiPedine;

    checkAndDisconnect(receive(game->players[game->turn], pila, sizeof(int)), game->players[game->turn]);
    checkAndDisconnect(receive(game->players[game->turn], numeroDiPedine, sizeof(int)), game->players[game->turn]);

    if (scegliPila(game, pila) != 0)
    {
        sendCode(game->players[game->turn], -2);
    }
    else if (mossa(game, numeroDiPedine) == -1)
    {
        sendCode(game->players[game->turn], -2);
    }
}

void terminaPartita(int player, int code, game_t *game)
{
    if (code == 0)
    {
        for (size_t i = 0; i < 2; i++)
        {
            if (game->players[i] == player)
            {
                sendMessage(game->players[i], "HAI VINTO!");
            }
            else
            {
                sendMessage(game->players[i], "HAI PERSO!");
            }
        }
    }
    else //Disconnessione
    {
        printf("Disconnetti");
        sendCode(player, 1);
    }
    
    close(game->players[0]);
    close(game->players[1]);

    free(game->field);
    free(game);
}

void sendCode(int fd, int code)
{
    checkAndDisconnect(sendSock(fd, &code, sizeof(int)), fd);
}

int checkAndDisconnect(int result, int fd)
{
    if (result == -1)
    {
        perror("Disconnected!");
        close(fd);
        return -1;
    }
    else
    {
        return fd;
    }
}