#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "tools.h"

int connectToServer(int domain, int type, int protocol, struct sockaddr *address, socklen_t len)
{
    int sock = socket(domain, type, protocol);
    checkWithExit(sock, -1, "socket()");

    checkWithExit(connect(sock, address, len), -2, "Connection failed\n");

    receiveMessage(sock, stdout);

    return sock;
}

void receiveMessage(int server, FILE *file)
{
    char *msg = malloc(sizeof(char));
    checkWithDisconnectAndExit(receive(server, msg, sizeof(char)), server, -3, "Receive message failed\n");
    fprintf(file, "%s", msg);
    fprintf(file, "%s", "\n");
}

int turno(field_t *field, int server)
{
    checkWithDisconnectAndExit(receive(server, field, sizeof(field_t)), server, -4, "turno()");
    printField(field, stdout);
    sendCommand(server, stdout, stdin, "Scegli la pila (0,1): ", "Errore: pila inserita non valida!\n");
    sendCommand(server, stdout, stdin,
                "Scegli il numero di pedine da rimuovere: ", "Errore: numero di pedine inserite non valido!\n");

    int serverCode = 0;
    checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, -3, "sendCommand()");
    return serverCode;
}

void sendCommand(int server, FILE *fileOutput, FILE *fileInput, const char *msg, const char *invalidInputMsg)
{
    int serverCode = 0;
    do
    {
        fprintf(fileOutput, "%s", msg);
        int x = 0;
        fscanf(fileInput, "%d", &x);
        checkWithDisconnectAndExit(sendSock(server, &x, sizeof(int)), server, -3, "sendCommand()");

        checkWithDisconnectAndExit(receive(server, &serverCode, sizeof(int)), server, -3, "sendCommand()");

        if (serverCode == -2)
        {
            fprintf(fileOutput, "%s", invalidInputMsg);
        }
    } while (serverCode == -2);
}

field_t *startGame(int server)
{
    int turn = 0;
    checkWithDisconnectAndExit(receive(server, &turn, sizeof(turn)), server, -2, "turn");

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
    if (result == -1)
    {
        close(server);
        checkWithExit(result, exitval, msg);
    }
}
