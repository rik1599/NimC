/**
 * tools.h
 * Funzioni di utilità per client e server
 */
#ifndef TOOLS_H
#define TOOLS_H

/**
 * Percorso del socket per la comunicazione
 */
#define SOCKADDR "/tmp/upperserver.socket"

/**
 * Riceve dal client/server secondo il seguente protocollo di comunicazione
 * 1) Viene inviata dal client la dimensione del messaggio in arrivo (in byte)
 * 2) Si incrementa la dimensione di buffer se minore della lunghezza del messaggio
 * 3) Si riceve il messaggio dal client
 * In caso di errori CHIUDE la comunicazione e ritorna -1
 * 
 * @param fd file descriptor della comunicazione in corso
 * @param buffer void* buffer in cui inserire il messaggio
 * 
 * @return -1 in caso di errori, 0 altrimenti
 */
int receive(int fd, void* buffer);

/**
 * Invia al client con lo stesso protocollo indicato sopra
 * 
 * @param fd file descriptor della comunicazione in corso
 * @param buffer void* buffer in cui inserire il messaggio
 * 
 * @return -1 in caso di errori, 0 altrimenti
 */
int send(int fd, void* message);

#endif