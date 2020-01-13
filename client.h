/**
 * Dichiarazioni delle funzioni per il funzionamento del client
 */
#ifndef CLIENT_H
#define CLIENT_H

#include "game.h"
#include <sys/socket.h>

/**
 * Crea un socket e lo connette al server. Gestisce eventuali
 * errori chiudendo il programma
 * @param domain dominio del socket (AF_LOCAL)
 * @param type tipo del socket (SOCK_STREAM)
 * @param protol protocollo da usare (0)
 * @param address indirizzo completo del socket
 *
 * @return file descriptor della connessione al server
 */
int connectToServer(int domain, int type, int protocol, struct sockaddr *address);

/**
 * Gestisce il turno lato client nel seguente modo:
 * 1) Riceve i messaggi con le istruzioni dal server
 * 2) Invia le proprie risposte (pila scelta, pedine da togliere)
 * 3) Ricezione dal server del field aggiornato
 * 4) Aggiornamento della visuale del giocatore
 *
 * @param field il terreno di gioco in uso (creato all'inizio della partita)
 */
int turno(field_t *field);

/**
 * Riceve i vari messaggi dal server
 * 
 * @param server file descriptor della connessione col server
 * 
 * @return messaggio ricevuto dal server
 */
char *receiveMessage(int server);

/**
 * Riceve i vari messaggi di inizio partita e il field iniziale
 */
field_t *startGame(int server);

#endif