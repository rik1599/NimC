/**
 * Dichiarazioni delle funzioni per il funzionamento del server
 */
#ifndef SERVER_H
#define SERVER_H

/**
 * Crea un socket, ne esegue il bind su address e imposta il limite
 * di connessioni simultanee (indicate dal programmatore) e gestisce eventuali 
 * errori chiudendo il programma
 * @param domain dominio del socket (AF_LOCAL)
 * @param type tipo del socket (SOCK_STREAM)
 * @param protol protocollo da usare(0)
 * @param address indirizzo completo del socket
 * @param limit numero massimo di connessioni contemporaneamente collegati al server
 */
int socketCreate(int domain, int type, int protocol, struct sockaddr *address, int limit);


int connectToClient(int socket);

#endif