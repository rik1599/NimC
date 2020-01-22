/**
 * tools.h
 * Funzioni di utilità per client e server
 */
#ifndef TOOLS_H
#define TOOLS_H

/**
 * Percorso del socket per la comunicazione
 */
#define SOCKADDR "/tmp/nimserver.socket"

/**
 * Riceve dal client/server secondo il seguente protocollo di comunicazione
 * 1) Viene inviata dal client la dimensione del messaggio in arrivo (in byte)
 * 2) Si incrementa la dimensione di buffer se minore della lunghezza del messaggio
 * 3) Si riceve il messaggio dal client
 * In caso di errori CHIUDE la comunicazione e ritorna -1
 *
 * @param fd file descriptor della comunicazione in corso
 * @param buffer void* buffer in cui inserire il messaggio/variabile (da castare nel tipo desiderato)
 * @param size dimensione del buffer
 *
 * @return -1 in caso di errori, 0 altrimenti
 */
int receive(int fd, void *buffer, unsigned int size);

/**
 * Invia al client con lo stesso protocollo indicato sopra
 *
 * @param fd file descriptor della comunicazione in corso
 * @param buffer void* buffer da cui inviare
 * @param size dimensione del buffer
 *
 * @return -1 in caso di errori, 0 altrimenti
 */
int sendSock(int fd, void *buffer, unsigned int size);

/*
 * Funzione di utilità per controllare il valore di ritorno di una funzione
 * POSIX e ritornare -1 in caso di errore
 */
int checkWithExit(int result, int exitval, const char *msg);

/*
 * Funzione di utilità per controllare il valore di ritorno di una funzione
 * POSIX e ritornare -1 in caso di errore
 */
int checkNoExit(int result, const char *msg);

#endif