/**
 * Struct e funzioni per il funzionamento del gioco
 */
#ifndef GAME_H
#define GAME_H

typedef struct field
{
    int pile[2];    // Il numero di pedine attualmente nella pila
    int pilascelta; // La pila scelta dal giocatore di turno (0,1)
} field_t;

typedef struct game
{
    int players[2]; // I giocatori (file descriptor)
    int turn;       // Il turno del giocatore (0,1)
    int turns;      // Contatore numero di turni
    field_t *field; // Il campo di gioco
} game_t;

/**
 * Crea il campo di gioco (estrae a caso il numero di pedine di ogni pila)
 *
 * @return una struct rappresentante il campo di gioco
 */
field_t *createField();

/**
 * Imposta la pila scelta dal giocatore di turno, se il numero dovesse essere diverso da (0,1)
 * torna -1
 * Se una pila è vuota e viene selezionata torna -2
 *
 * @param game il gioco in corso
 * @param pila la pila da scegliere (0,1)
 */
int scegliPila(game_t *game, unsigned int pila);

/**
 * Rimuove le pedine dalla pila scelta dal giocatore
 * Se il numero di pedine inserito dovesse essere maggiore di quelle rimaste nella pila scelta
 * torna -1
 * Se una delle due pile dovesse essere vuota e nell'altra sono presenti ancora pedine,
 * il numero massimo di pedine che possono essere tolte è il numero delle pedine presenti - 1
 * (vince chi toglie l'ultima pedina)
 *
 * @param game il gioco in corso
 * @param pedine il numero di pedine da rimuovere dalla pila scelta in precedenza
 */
int mossa(game_t *game, unsigned int pedine);

/**
 * Controlla se è presente al più una pedina tra tutte e due le pile
 * Se è vero torna il giocatore di turno che viene dichiarato vincitore
 * Se il numero delle pedine è diverso da >=1 ritorna 2.
 */
int winner(game_t *game);

/**
 * Cambia il turno tra i giocatori
 */
void cambiaTurno(game_t *game);

#endif