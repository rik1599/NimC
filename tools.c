#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int receive(int fd, void *buffer, unsigned int size)
{
    int newSize = 0;
    if (check(recv(fd, &newSize, sizeof(int), 0), "size not received") == -1)
    {
        return -1;
    }

    if (newSize > size)
    {
        buffer = realloc(buffer, newSize);
        size = newSize;
    }

    if (check(recv(fd, buffer, size, 0), "data not received") == -1)
    {
        return -1;
    }

    return 0;
}

int sendSock(int fd, void *buffer, unsigned int size)
{
    if (check(send(fd, &size, sizeof(int), 0), "size not sent") == -1)
    {
        return -1;
    }

    if (check(send(fd, buffer, size, 0), "data not sent") == -1)
    {
        return -1;
    }

    return 0;
}

/*
 * Funzione di utilità per controllare il valore di ritorno di una funzione
 * POSIX e ritornare -1 in caso di errore
 */
int check(int result, const char *msg)
{
    if (result == -1)
    {
        perror(msg);
    }
    return result;
}