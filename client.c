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

    checkWithExit(connect(sock, address, sizeof(address)), -2, "Connection failed");

    receiveMessage(sock, stdout);

    return sock;
}

void receiveMessage(int server, FILE *file)
{
    char *msg = malloc(sizeof(char));
    checkWithDisconnectAndExit(receive(server, msg, sizeof(char)), server, -3, "Receive message failed");
    fprintf(file, "%s" ,msg);
    fprintf(file, "%s", "\n");
}

int turno(field_t *field, int server)
{
    checkWithDisconnectAndExit(receive(server, field, sizeof(field_t)), server, -4, "turno()");
    printField(field, stdout);
    sendCommand(server, stdin, "Scegli la pila (0,1): ", "Errore: pila inserita non valida!");
    sendCommand(server, stdin,
                "Scegli il numero di pedine da rimuovere: ", "Errore: numero di pedine inserite non valido!");

    int serverCode = 0;
    checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, -3, "sendCommand()");
    return serverCode;
}

void sendCommand(int server, FILE *file, const char *msg, const char *invalidInputMsg)
{
    fprintf(file, "%s",msg);
    int x = 0;
    fprintf(file, "%d", x);
    checkWithDisconnectAndExit(sendSock(server, &x, sizeof(int)), server, -3, "sendCommand()");

    int serverCode = 0;
    checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, -3, "sendCommand()");

    if (serverCode == -2)
    {
        fprintf(file, "%s", invalidInputMsg);
        sendCommand(server, file, msg, invalidInputMsg);
    }
}

field_t *startGame(int server)
{
    int turn = 0;
    checkWithDisconnectAndExit(receive(server, &turn, sizeof(turn)), server, -2, "turn");

    field_t *field = malloc(sizeof(field_t));
    checkWithDisconnectAndExit(receive(server, field, sizeof(field_t)), server, -2, "field_t");

    fprintf(stdout, "Partita iniziata");

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
        fprintf(file, "\n");
    }
}

void checkWithDisconnectAndExit(int result, int server, int exitval, const char *msg)
{
    if (result == -1)
    {
        close(server);
        checkWithExit(result, exitval, msg);
    }
}
