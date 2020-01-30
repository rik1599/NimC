/**
 * Struct e funzioni per il funzionamento del gioco
 */
#ifndef GAME_H
#define GAME_H

#define NO_WINNER -5
#define INVALID_RANGE -6

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
 * Imposta la pila scelta dal giocatore di turno
 *
 * @param game il gioco in corso
 * @param pila la pila da scegliere (0,1)
 *
 * @return INVALID_RANGE se il numero è diverso da (0,1) o se selezionata una pila vuota, 0 altrimenti
 *
 */
int scegliPila(game_t *game, unsigned int pila);

/**
 * Se una delle pile è stata già svuotata, controlla il numero massimo di pedine che possono essere inserite
 *
 * @param game la partita in corso
 *
 * @return numero massimo di pedine per la pila restante
 *
 */
int getMaxPedine(game_t *game);

/**
 * Rimuove le pedine dalla pila scelta dal giocatore
 * Se il numero di pedine inserito dovesse essere maggiore di quelle rimaste nella pila scelta
 * torna INVALID_RANGE
 * Se una delle due pile dovesse essere vuota e nell'altra sono presenti ancora pedine,
 * il numero massimo di pedine che possono essere tolte è il numero delle pedine presenti - 1
 * (vince chi toglie l'ultima pedina)
 *
 * @param game il partita in corso
 * @param pedine il numero di pedine da rimuovere dalla pila scelta in precedenza
 *
 * @return 0 se non ci sono errori, INVALID_RANGE altrimenti
 */
int mossa(game_t *game, unsigned int pedine);

/**
 * Controlla se è presente al più una pedina tra tutte e due le pile
 *
 * @param game la partita in corso
 *
 * @return Se è vero torna il giocatore di turno che viene dichiarato vincitore, altimenti
 * se il numero delle pedine è >1 ritorna NO_WINNER.
 */
int winner(game_t *game);

/**
 * Cambia il turno tra i giocatori
 */
void cambiaTurno(game_t *game);

#endif
