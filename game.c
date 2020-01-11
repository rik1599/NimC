#include "game.h"
#include <stdlib.h>
#include <time.h>

/**
 * Crea il campo di gioco (estrae a caso il numero di pedine di ogni pila)
 *
 * @return una struct rappresentante il campo di gioco
 */
field_t *createField()
{

    int numElementiPilaUno;
    int numElementiPilaDue;
    time_t t;

    srand((unsigned)time(&t));

    // Numeri di elementi delle due pile
    numElementiPilaUno = rand() % 20;
    numElementiPilaDue = rand() % 20;

    // Creazione del campo di gioco
    field_t *campoDiGioco = malloc(sizeof(field_t));

    // Assegnazione del numero di pendine in ogni pila
    campoDiGioco->pile[0] = numElementiPilaUno;
    campoDiGioco->pile[1] = numElementiPilaDue;

    return campoDiGioco;
}

/**
 * Imposta la pila scelta dal giocatore di turno, se il numero dovesse essere
 * diverso da (0,1) torna -1 Se una pila è vuota e viene selezionata torna -2
 *
 * @param game il gioco in corso
 * @param pila la pila da scegliere (0,1)
 */
int scegliPila(game_t *game, unsigned int pila)
{

    if (pila != 0 || pila != 1)
    {
        return -1;
    }
    else if (game->field->pile[game->field->pilascelta] == 0)
    {
        return -2;
    }
    else
    {
        game->field->pilascelta = pila;
    }

    return 0;
}

/**
 * Rimuove le pedine dalla pila scelta dal giocatore
 * Se il numero di pedine inserito dovesse essere maggiore di quelle rimaste
 * nella pila scelta torna -1 Se una delle due pile dovesse essere vuota e
 * nell'altra sono presenti ancora pedine, il numero massimo di pedine che
 * possono essere tolte è il numero delle pedine presenti - 1 (vince chi toglie
 * l'ultima pedina)
 *
 * @param game il gioco in corso
 * @param pedine il numero di pedine da rimuovere dalla pila scelta in
 * precedenza
 */
int mossa(game_t *game, unsigned int pedine)
{

    if (game->field->pile[game->field->pilascelta] < pedine)
    {
        return -1;
    }

    return 0;
}

/**
 * Controlla se è presente al più una pedina tra tutte e due le pile
 * Se è vero torna il giocatore di turno che viene dichiarato vincitore
 * Se il numero delle pedine è diverso da >=1 ritorna 2.
 */
int winner(game_t *game)
{
    int pedineRimaste;

    pedineRimaste = game->field->pile[0] + game->field->pile[1];

    if (pedineRimaste >= 1)
    {
        return game->turn;
    }

    return 2;
}

/**
 * Cambia il turno tra i giocatori
 */
void cambiaTurno(game_t *game)
{

    switch (game->turn)
    {
    case 0:
        game->turn = 1;
        break;
    case 1:
        game->turn = 0;
        break;
    }
}