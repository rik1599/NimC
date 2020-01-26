#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int receive(int fd, void *buffer, unsigned int size)
{
    // Se viene ricevuta una dimensione 0 c'è stata una disconnessione
    int newSize = 0;
    if (checkNoExit(recv(fd, &newSize, sizeof(int), 0), "size not received") <= 0)
    {
        return DISCONNECTED;
    }

    if (newSize > size)
    {
        buffer = realloc(buffer, newSize);
        size = newSize;
    }

    if (checkNoExit(recv(fd, buffer, size, 0), "data not received") <= 0)
    {
        return DISCONNECTED;
    }

    return 0;
}

int sendSock(int fd, void *buffer, unsigned int size)
{   
    if (checkNoExit(send(fd, &size, sizeof(int), 0), "size not sent") < 0)
    {
        return DISCONNECTED;
    }

    if (checkNoExit(send(fd, buffer, size, 0), "data not sent") < 0)
    {
        return DISCONNECTED;
    }

    return 0;
}

int checkWithExit(int result, int exitval, const char *msg)
{
    if (result < 0)
    {
        perror(msg);
        exit(exitval);
    }
    return result;
}

int checkNoExit(int result, const char *msg)
{
    if (result < 0)
    {
        perror(msg);
    }
    return result;
}