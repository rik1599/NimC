/**
 * Dichiarazioni delle funzioni per il funzionamento del server
 */
#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

/**
 * Crea un socket, ne esegue il bind su address e imposta il limite
 * di connessioni simultanee (indicate dal programmatore) e gestisce eventuali 
 * errori chiudendo il programma
 * @param domain dominio del socket (AF_LOCAL)
 * @param type tipo del socket (SOCK_STREAM)
 * @param protol protocollo da usare (0)
 * @param address indirizzo completo del socket
 * @param limit numero massimo di connessioni contemporaneamente collegati al server
 * 
 * @return socket descriptor
 */
int createAndBind(int domain, int type, int protocol, struct sockaddr *address, int limit);

/**
 * Si connette a un client
 * In caso di errori esce dalla funzione segnalando l'errore
 * (Non deve bloccare il programma)
 * @param socket un binded socket
 * 
 * @return file descriptor connesso al client, -1 se ERRORE
 */
int connectToClient(int socket);

/**
 * Riceve dal client secondo il seguente protocollo di comunicazione
 * 1) Viene inviata dal client la dimensione del messaggio in arrivo (in byte)
 * 2) Viene istanziato un buffer puntato a void* della dimensione ricevuta
 * 3) Si riceve il messaggio dal client
 * In caso di errori CHIUDE la comunicazione e ritorna -1 come puntatore a int*
 * 
 * @param fd file descriptor della comunicazione in corso
 * @return void* buffer con il messaggio (da castare nel tipo desiderato), -1* se ERRORE
 */
void* receive(int fd);

#endif