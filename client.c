#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "tools.h"

int connectToServer(int domain, int type, int protocol, struct sockaddr *address, socklen_t len)
{
    int sock = socket(domain, type, protocol);
    checkWithExit(sock, INVALID_SOCKET, "socket()");

    checkWithExit(connect(sock, address, len), INVALID_SOCKET, "Connection failed\n");

    receiveMessage(sock, stdout);

    return sock;
}

void receiveMessage(int server, FILE *file)
{
    char *msg = malloc(sizeof(char));
    checkWithDisconnectAndExit(receive(server, msg, sizeof(char)), server, DISCONNECTED, "Receive message failed\n");
    fprintf(file, "%s", msg);
    fprintf(file, "%s", "\n");
}

int getWinner(int server)
{
    int serverCode = 0;
    checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, DISCONNECTED, "sendCommand()");
    return serverCode;
}

int turno(field_t *field, int server, int turno)
{
    // A turno 1 non controlla se ci sono vincitori
    if (turno != 1)
    {
        int winner = getWinner(server);
        if (winner != NO_WINNER)
        {
            return winner;
        }
    }
    
    checkWithDisconnectAndExit(receive(server, field, sizeof(field_t)), server, DISCONNECTED, "turno()");
    printField(field, stdout);
    sendClientInput(server, stdout, stdin, "Scegli la pila (0,1): ", "Errore: pila inserita non valida!\n");
    sendClientInput(server, stdout, stdin,
                "Scegli il numero di pedine da rimuovere: ", "Errore: numero di pedine inserite non valido!\n");
    fprintf(stdout, "Turno finito, tocca al tuo avversario\n");
    return getWinner(server);
}

void sendClientInput(int server, FILE *fileOutput, FILE *fileInput, const char *msg, const char *invalidInputMsg)
{
    int serverCode = 0;
    do
    {
        fprintf(fileOutput, "%s", msg);
        int x = 0;
        fscanf(fileInput, "%d", &x);
        checkWithDisconnectAndExit(sendSock(server, &x, sizeof(int)), server, DISCONNECTED, "sendMessageInput()");

        checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, DISCONNECTED, "sendMessageInput()");

        if (serverCode == INVALID_RANGE)
        {
            fprintf(fileOutput, "%s", invalidInputMsg);
        }
    } while (serverCode == INVALID_RANGE);
}

field_t *startGame(int server)
{
    int turn = 0;
    checkWithDisconnectAndExit(receive(server, &turn, sizeof(turn)), server, DISCONNECTED, "turn");

    field_t *field = malloc(sizeof(field_t));

    fprintf(stdout, "Partita iniziata\n");

    if (turn == 0)
    {
        fprintf(stdout, "Inizi tu!\n");
    }

    return field;
}

void printField(field_t *field, FILE *file)
{
    for (int i = 0; i < 2; i++)
    {
        fprintf(file, "Pila %d:", i);
        for (size_t j = 0; j < field->pile[i]; j++)
        {
            fprintf(file, "| ");
        }
        fprintf(file, "(%d)\n", field->pile[i]);
    }
}

void checkWithDisconnectAndExit(int result, int server, int exitval, const char *msg)
{
    if (result == ERROR)
    {
        close(server);
        checkWithExit(result, exitval, msg);
    }
}
