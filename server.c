#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

#include "server.h"
#include "tools.h"

int createAndBind(int domain, int type, int protocol, struct sockaddr *address, socklen_t len, int limit)
{
    int sock = socket(domain, type, protocol);
    checkWithExit(sock, 1, "socket()");
    checkWithExit(bind(sock, address, len), 2, "bind()");
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
            if(sendMessageToSock(fd, "Connesso!, In attesa di un altro giocatore...") == 0)
            {
                fprintf(stderr, "Giocatore connesso\n");
            }

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
    newGame->turns = 0;

    fprintf(stderr, "Giocatore 1: %d\nGiocatore 2: %d\nPila 1: %d\nPila 2: %d\n", newGame->players[0],
            newGame->players[1], newGame->field->pile[0], newGame->field->pile[1]);

    return newGame;
}

void *playGame(void *arg)
{
    fprintf(stderr, "Partita iniziata su thread id = %d\n", (int)pthread_self());
    game_t *game = (game_t *)arg;
    for (int i = 0; i < 2; i++)
    {
        sendCode(game->players[i], i, game);
    }

    int winPlayer = winner(game);
    while (winPlayer == 2)
    {
        checkAndDisconnect(sendSock(game->players[game->turn], game->field, sizeof(field_t)), game);
        turno(game);
        winPlayer = winner(game);
        sendCode(game->players[game->turn], winPlayer, game);
        cambiaTurno(game);

        if (game->turns > 0)
        {
            sendCode(game->players[game->turn], winPlayer, game);
        }

        game->turns++;    
    }

    terminaPartita(winPlayer, 0, game);

    return NULL;
}

int sendMessageToSock(int fd, char *message)
{
    int size = (strlen(message) * sizeof(char));
    return sendSock(fd, message, size);
}

void sendMessage(int player, char *message, game_t *game)
{
    checkAndDisconnect(sendMessageToSock(player, message), game);
}

void turno(game_t *game)
{
    int pila;
    int numeroDiPedine;

    int checkPila = 0;
    do
    {
        checkAndDisconnect(receive(game->players[game->turn], (void *)&pila, sizeof(int)), game);
        checkPila = scegliPila(game, pila);
        if (checkPila != 0)
        {
            sendCode(game->players[game->turn], -2, game);
        }
        else
        {
            sendCode(game->players[game->turn], 0, game);
        }

    } while (checkPila != 0);

    int checkPedine = 0;
    do
    {
        checkAndDisconnect(receive(game->players[game->turn], (void *)&numeroDiPedine, sizeof(int)),
                           game);
        checkPedine = mossa(game, numeroDiPedine);
        if (checkPedine == -1)
        {
            sendCode(game->players[game->turn], -2, game);
        }
        else
        {
            sendCode(game->players[game->turn], 0, game);
        }

    } while (checkPedine == -1);
}

void terminaPartita(int player, int code, game_t *game)
{
    if (code == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            if (game->players[i] == player)
            {
                sendMessage(game->players[i], "HAI VINTO!", game);
            }
            else
            {
                sendMessage(game->players[i], "HAI PERSO!", game);
            }
        }
    }

    closeThread(game);
}

void closeThread(game_t *game)
{
    close(game->players[0]);
    close(game->players[1]);
    free(game->field);
    free(game);
    fprintf(stderr, "Partita terminata\n");
    pthread_exit(NULL);
}

void sendCode(int fd, int code, game_t *game)
{
    checkAndDisconnect(sendSock(fd, (void *)&code, sizeof(int)), game);
}

void checkAndDisconnect(int result, game_t *game)
{
    if (result == -1)
    {
        perror("Disconnected!");
        closeThread(game);
    }
}