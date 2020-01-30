/**
 * Dichiarazioni delle funzioni per il funzionamento del client
 */
#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <stdio.h>

#include "game.h"

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
int connectToServer(int domain, int type, int protocol, struct sockaddr *address, socklen_t len);

/**
 * Gestisce il turno lato client nel seguente modo:
 * 1) Riceve i messaggi con le istruzioni dal server
 * 2) Invia le proprie risposte (pila scelta, pedine da togliere)
 * 3) Ricezione dal server del field aggiornato
 * 4) Aggiornamento della visuale del giocatore
 *
 * @param field il campo di gioco in uso (creato all'inizio della partita)
 * @param server file descriptor della connessione col server
 * @param turno 1 se è il primo turno del giocatore, 0 altrimenti
 * 
 * @return l'intero corrispondente al player vincitore
 */
int turno(field_t *field, int server, int turno);

/**
 * Riceve i vari messaggi dal server e li stampa su stdout
 * 
 * @param server file descriptor della connessione col server
 * @param file file descriptor (stdout)
 * 
 */
void receiveMessage(int server, FILE *file);

/**
 * Riceve i vari messaggi di inizio partita e il field iniziale
 * 
 * @param server server file descriptor
 * 
 * @return il campo di gioco creato all'inizio
 */
field_t *startGame(int server);

/**
 * Stampa il field su file
 * 
 * @param field campo di gioco in uso
 * @param file file descriptor (stdout) 
 * 
 */
void printField(field_t *field, FILE *file);

/**
 * Legge gli input del client e gli invia al server
 * @param server server file descriptor
 * @param fileOutput file description (stdout)
 * @param fileInput file description (stdin)
 * @param msg messaggio da stampare
 * @param invalidInputMsg messaggio di errore
 * 
 */
void sendClientInput(int server, FILE *fileOutput, FILE *fileInput, const char *msg, const char *invalidInputMsg);


/**
 * Controlla se c'è un errore, se è presente chiude la partita ed esce
 * @param result risultato da controllare
 * @param server file descriptor del server
 * @param exitval codice dell'errore
 * @param msg messaggi dell'errore
 * 
 */
void checkWithDisconnectAndExit(int result, int server, int exitval, const char *msg);

/**
 * Riceve il codice del vincitore dal server
 * @param server file descriptor della connessione al server
 * 
 * @return NO_WINNER se non ci sono vincitori, altro intero altrimenti
 */
int getWinner(int server);
#endif