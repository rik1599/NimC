/**
 * Dichiarazioni delle funzioni per il funzionamento del server
 */
#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include <sys/socket.h>
#include <stdio.h>

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
int createAndBind(int domain, int type, int protocol, struct sockaddr *address, socklen_t len, int limit);

/**
 * Si connette a un client
 * In caso di errori esce dalla funzione segnalando l'errore
 * (Non deve bloccare il programma)
 * @param socket un binded socket
 *
 * @return file descriptor connesso al client, -1 se ERRORE
 */
int connectToPlayer(int socket);

/**
 * Dato un campo di gioco creato in precedenza crea un nuovo
 * @param playerOne socket descriptor del giocatore 1
 * @param playerTwo socket descriptor giocatore 2
 * @param field il campo di gioco creato precedentemente
 *
 * @return una struct che rappresenta la partita
 */
game_t *iniziaPartita(int playerOne, int playerTwo, field_t *field);

/**
 * Funzione che gestisce la partita (da chiamare in fase di creazione del thread)
 * 
 * @param arg struct della partita (passata come void * causa chiamata pthread_create)
 * @return null pointer
 */
void *playGame(void *arg);

int sendMessageToSock(int fd, char *message);

/**
 * Invia al player attualmente attivo i vari messaggi di output
 * In caso di errore disconnette
 * @param player il giocatore a cui inviare il messaggio
 * @param message il messaggio da visualizzare
 */
void sendMessage(int player, char *message, game_t *game);

/**
 * Gestisce il turno lato server nel seguente modo
 * 1) Ricezione della pila scelta
 * 2) Ricezione del numero di pedine da rimuovere
 * Se in queste operazioni dovessero esserci delle irregolarità, viene richiesto al giocatore
 * di turno di reinserire la scelta sbagliata
 * 3) Applicazione delle mosse e invio del field aggiornato
 * 4) Cambio turno tra i giocatori
 * 5) Controllo del vincitore
 *
 * In caso di errori di comunicazione termina la partita
 *
 * @param game la partita in corso
 */
void turno(game_t *game);

/**
 * Termina la partita disconnettendo i giocatori
 * Se la partita è terminata causa disconnessione (-1) il giocatore ancora online è dichiarato vincitore
 * (ma viene comunque segnalata la disconnessione)
 * 
 * @param player il giocatore vincitore
 * @param code il motivo dell'interruzione (0 partita finita, -1 disconnessione o errori vari)
 * @param game_t *game il gioco (fare i free)
 */
void terminaPartita(int player, int code, game_t *game);

void closeThread(game_t *game);

/**
 * 
 */
void checkAndDisconnect(int result, game_t *game);

/**
 * Invia al giocatore un 
 */
void sendCode(int fd, int code, game_t *game);

#endif