#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

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
            sendMessage(fd, "Connesso!, In attesa di un altro giocatore...");
            fprintf(stderr, "Giocatore connesso\n");

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

    fprintf(stderr,
        "Giocatore 1: %d\nGiocatore 2: %d\nPila 1: %d\nPila 2: %d\n",
        newGame->players[0], newGame->players[1], newGame->field->pile[0], newGame->field->pile[1]);

    return newGame;
}

void *playGame(void *arg)
{
    fprintf(stderr, "Partita iniziata su thread id = %d\n", (int) pthread_self());
    game_t *game = (game_t *)arg;
    for (int i = 0; i < 2; i++)
    {
        sendCode(game->players[i], i);
        //checkAndDisconnect(sendSock(game->players[i], game->field, sizeof(field_t)), -2);
    }

    int winPlayer = winner(game);
    while (winPlayer == 2)
    {
        checkAndDisconnect(sendSock(game->players[game->turn], game->field, sizeof(field_t)), -2);
        turno(game);
        winPlayer = winner(game);
        sendCode(game->players[game->turn], winPlayer);
        cambiaTurno(game);
    }

    terminaPartita(winPlayer, 0, game);

    return NULL;
}

void sendMessage(int player, char *message)
{
    int size = (strlen(message) * sizeof(char));
    checkAndDisconnect(sendSock(player, message, size), player);
}

void turno(game_t *game)
{
    int pila;
    int numeroDiPedine;

    int checkPila = 0;
    do
    {
        checkAndDisconnect(receive(game->players[game->turn], (void *) &pila, sizeof(int)), game->players[game->turn]);
        checkPila = scegliPila(game, pila);
        if (checkPila != 0)
        {
            sendCode(game->players[game->turn], -2);
        } 
        else
        {
            sendCode(game->players[game->turn], 0);
        }
        
        
    } while (checkPila != 0);
    
    int checkPedine = 0;
    do
    {
        checkAndDisconnect(receive(game->players[game->turn], (void *) &numeroDiPedine, sizeof(int)), game->players[game->turn]);
        checkPedine = mossa(game, numeroDiPedine);
        if (checkPedine == -1)
        {
            sendCode(game->players[game->turn], -2);
        }
        else
        {
            sendCode(game->players[game->turn], 0);
        }
        
        
    } while (checkPedine == -1);
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
    checkAndDisconnect(sendSock(fd, (void *) &code, sizeof(int)), fd);
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